#include "Server.h"

Server::Server()
{
    part = new Database;
    aut = new identification(part);
    world = new World;

    connect(aut, SIGNAL(socketWrite(QByteArray, int)), this, SLOT(socketWrite(QByteArray, int)));
    connect(world, SIGNAL(socketStructWrite(Avatar*, Client*)), this, SLOT(socketStructWrite(Avatar*, Client*)));
    connect(world, SIGNAL(socketWrite(QByteArray, int)), this, SLOT(socketWrite(QByteArray, int)));

    UdpSocket = new QUdpSocket(this); //в данном случае мы сервер
    UdpSocket->bind(QHostAddress::Any, 5556);
    connect(UdpSocket, SIGNAL(readyRead()), this, SLOT(sockUdpReady()));
}

Server::~Server()
{
    delete part;
    delete aut;
    delete world;

    for (int j = 0; j < clients.size(); j++)
        delete clients[j];
    for (int j = 0; j < maps.size(); j++)
        delete maps[j];
}

bool Server::LaunchServer()
{
    if (this->listen(QHostAddress::Any, 5555)) //хост, порт
    {
        part->db = new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL"));
        part->db->setHostName("127.0.0.1"); //localhost
        part->db->setUserName("root");
        part->db->setPassword("root");
        part->db->setDatabaseName("rpg");

        if (part->db->open()) {
            qDebug() << "listening";
            return true;
        }
        else {
            qDebug() << " NOSQL ";
            QMessageBox::information(NULL, "error","NOSQL",NULL, NULL);
            this->close(); //закрываем доступ к серверу
            return false;
        }

    } else {
        qDebug() << "No listening";
        return false;
    }
}

void Server::DeleteClient()
{
    for (int c = 0; c < clients.size(); c++)
        if (clients[c]->socket->state() != QAbstractSocket::ConnectedState) { //поиск отключенного сокета

            //рассылаем все клиентам запросы о отключении игрока
            QByteArray itog = "{\"Protocol\":\"Unconnect\",\"p_id\":\"";
            itog.append(QString::number(clients[c]->avatar.pid));
            itog.append("\"}");

            for (int j = 0; j < clients.size(); j++)
                clients[j]->socket->write(itog, itog.length());

            //далее удаляем клиента
            qDebug() << "Disconnect ";

            emit DeleteClientList(c); //удаление строки в списке
            clients[c]->socket->deleteLater(); //удаление сокета
            world->DeleteClient(clients[c]); //удаление игрока из мира

            if (clients[c]->myMap != NULL) {  //удаление игрока из карты
                clients[c]->myMap->DeleteClient(clients[c]);
                if (clients[c]->myMap->player.size() == 0) { //удаление карты если вн.модуля нет
                    delete clients[c]->myMap;
                    maps.removeOne(clients[c]->myMap);
                }
            }

            clients.remove(c); //удаление игрока из вектора
        }
}


void Server::incomingConnection(int socketDescriptor)
{
    clients.push_back(new Client);
    clients.back()->socket = new QTcpSocket(this);
    clients.back()->socket->setSocketDescriptor(socketDescriptor); //присвоить сокету номер
    clients.back()->avatar.socketDescriptor = socketDescriptor; //присвоить аватару номер
    clients.back()->myMap = NULL;

    connect(clients.back()->socket, SIGNAL(readyRead()), this, SLOT(sockReady()));
    connect(clients.back()->socket, SIGNAL(disconnected()), this, SLOT(DeleteClient()));

    clients.back()->socket->write("\"Connect\":\"true\",\"Descriptor\":\""
                                 + QByteArray::number(socketDescriptor) + "\"");
    clients.back()->socket->waitForConnected(500);
}


void Server::sockUdpReady()
{
    //принятие udp запросов
    int NP = 0; //numberPlayer (Avatar)
    QHostAddress sender; //адресс
    quint16 senderPort; //порт

    if (UdpSocket->hasPendingDatagrams()) //прием структур данных
    {
        QByteArray data; //основной формат приема данных
        data.resize(UdpSocket->pendingDatagramSize());

        UdpSocket->readDatagram(data.data(), data.size(), &sender, &senderPort);


        //------------------------------------------------------ Принятие json udp
        doc = QJsonDocument::fromJson(data.data(), &docError);

        if (docError.errorString() == "no error occurred") { //если запрос типа json

            if (doc.object().value("Protocol") == "Attack") { //запрос о нанесении урона
                QString p_id_ = doc.object().value("p_id").toString();
                QString damage = doc.object().value("damage").toString();
                unsigned int p_id = p_id_.toInt();

                //world->Damage(p_id, damage.toInt()); //нанесение урона
                for (int j = 0; j < maps.size(); j++) //нанесение урона
                    maps[j]->Damage(p_id, damage.toInt()); //запрос для всех карт (в будущем протокол будет изменен)
            }

            if (doc.object().value("Protocol") == "ShowAttack")
            {
                for (int j = 0; j < clients.size(); j++)
                    if (clients[j]->senderPort != senderPort)
                        UdpSocket->writeDatagram(data, data.length(), clients[j]->sender, clients[j]->senderPort);
            }

        } else { //либо если это не тип json то это аватар
        //---------------------------------------------------------

            Avatar* av = ((Avatar*)(data.data()));

            for (int j = 0; j < clients.size(); j++)
               if (av->socketDescriptor == clients[j]->avatar.socketDescriptor) { //по дескриптору ищем номер аватара
                   clients[j]->avatar = *av;
                   NP = j; break;
               }

            clients[NP]->senderPort = senderPort;
            clients[NP]->sender = sender;

            for (int j = 0; j < maps.size();j++) //каждый запрос распределяем на сервер
                if (clients[NP]->myMap == maps[j]) {
                    maps[j]->Replication(NP);
                    if (maps[j]->CheckUdp(av)) return;
                }
        }
    }

}


void Server::sockReady()
{
    int NS = 0; //numberSocket

    for (int j = 0; j < clients.size(); j++)
        if (clients[j]->socket == qobject_cast< QTcpSocket* >(sender())) //святая строчка проверки
            NS = j; //кто испускает сигнал


    Data = clients[NS]->socket->readAll(); //берем запрос
    doc = QJsonDocument::fromJson(Data, &docError); //перевод из формата json
    part->query = QSqlQuery(*part->db); //перевод в query базы данных

    qDebug() << "запрос " << Data;

    ///////////////////////////////////////////////////////////////////////////////
    //Обработка протоколов

    if (doc.object().value("protocol").toString() == "identefication")
        aut->identefication_(doc, clients[NS]->socket->socketDescriptor(), clients); //протокол идентефикации


    //if (doc.object().value("protocol").toString() == "endConnect") //протокол конца соединения
    //    aut->db_reception(); //прием данных


    if (doc.object().value("protocol").toString() == "EnterTheWorld") { //запрос на вход в мир
        if (doc.object().value("type").toString() == "Enter") {

         world->newClient(clients[NS]); //добавляем клиента в общий мир
         QString nameMap = doc.object().value("name").toString();
         bool ka = false; //игрок первый в мире?

         for (int j = 0; j < maps.size(); j++)
             if (maps[j]->nameMap == nameMap) { //добавляем его в мир в который он зашел
                 maps[j]->player.push_back(clients[NS]);
                 clients[NS]->myMap = maps[j];
                 break;
             }
             else if (maps.size() - 1)
                 ka = true;
         if (maps.size() == 0) ka = true;
         if (ka)
         {
             maps.push_back(new Serv01Map(clients.back(), world, doc.object().value("name").toString()));
             maps.back()->player.push_back(clients[NS]);
             clients[NS]->myMap = maps.back();

             connect(maps.back(), SIGNAL(socketStructWrite(Avatar*, Client*)), this, SLOT(socketStructWrite(Avatar*, Client*)));
             connect(maps.back(), SIGNAL(socketWrite(QByteArray, int)), this, SLOT(socketWrite(QByteArray, int)));
         }
       }

        if (doc.object().value("type").toString() == "Exit") //выход игрока из мира
            for (int j = 0; j < maps.size(); j++)
                if (maps[j]->nameMap == doc.object().value("name").toString())
                { maps[j]->DeleteClient(clients[NS]); break; }

        if (doc.object().value("type").toString() == "successful") //отправка 2 сегмента
            socketWrite("{\"Protocol\":\"SC\"}", clients[NS]->avatar.socketDescriptor);
    }


    Data = "";
}


////////////////////////////////////////////////////////////////////////////////////////////
///------------сигналы


void Server::socketWrite(QByteArray str, int socketDescriptor) //отправка данных по TCP
{
    for (int j = 0; j < clients.size(); j++)
        if (clients[j]->socket->socketDescriptor() == socketDescriptor)
            clients[j]->socket->write(str); //отправить сообщение выбранному сокету
}

void Server::socketStructWrite(Avatar* av, Client* client) //отправка структур данные через UDP запрос
{
    UdpSocket->writeDatagram(reinterpret_cast<char*>(av), 256, client->sender, client->senderPort);
}

