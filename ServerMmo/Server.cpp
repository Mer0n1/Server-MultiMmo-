#include "Server.h"

Server::Server()
{
    part = new Database;
    aut = new identification(part);
    world = new World;

    connect(aut, SIGNAL(socketWrite(QByteArray, int)), this, SLOT(socketWrite(QByteArray, int)));
    connect(world, SIGNAL(socketStructWrite(Avatar*, Client*)), this, SLOT(socketStructWrite(Avatar*, Client*)));

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
            QByteArray itog = "{\"type\":\"Unconnect\",\"p_id\":\"";
            itog.append(QString::number(clients[c]->avatar.p_id));
            itog.append("\"}");

            for (int j = 0; j < clients.size(); j++)
                clients[j]->socket->write(itog, itog.length());

            //далее удаляем клиента
            qDebug() << "Disconnect ";

            emit DeleteClientList(c); //удаление строки в списке
            clients[c]->socket->deleteLater(); //удаление сокета
            world->DeleteClient(clients[c]); //удаление игрока из мира
            clients.remove(c); //удаление игрока из вектора
        }
}


void Server::incomingConnection(int socketDescriptor)
{
    clients.push_back(new Client);
    clients.back()->socket = new QTcpSocket(this);
    clients.back()->socket->setSocketDescriptor(socketDescriptor); //присвоить сокету номер
    clients.back()->avatar.socketDescriptor = socketDescriptor; //присвоить аватару номер

    connect(clients.back()->socket, SIGNAL(readyRead()), this, SLOT(sockReady()));
    connect(clients.back()->socket, SIGNAL(disconnected()), this, SLOT(DeleteClient()));

    clients.back()->socket->write("You connected. You number ID-sock: "); //отправляем номер дескриптора
    clients.back()->socket->write(QByteArray::number(socketDescriptor)); //передать чистый int
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

            if (doc.object().value("type") == "Attack") { //запрос о нанесении урона
                QString p_id_ = doc.object().value("p_id").toString();
                QString damage = doc.object().value("damage").toString();
                unsigned int p_id = p_id_.toInt();

                world->Damage(p_id, damage.toInt()); //нанесение урона
            }


        } else { //либо если это не тип json то это аватар
        //---------------------------------------------------------

            int DeskSock = ((Avatar*)(data.data()))->socketDescriptor; //по дескриптору ищем номер аватара

            for (int j = 0; j < clients.size(); j++)
               if (DeskSock == clients[j]->avatar.socketDescriptor) {
                   clients[j]->avatar = *((Avatar*)(data.data()));
                   NP = j; break;
               }

            clients[NP]->senderPort = senderPort;
            clients[NP]->sender = sender; //был back

            world->Replication(NP); //вызов репликации
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

    if (doc.object().value("protocol").toString() == "identefication")
        aut->identefication_(doc, clients[NS]->socket->socketDescriptor(), clients); //протокол идентефикации

    //if (doc.object().value("protocol").toString() == "endConnect") //протокол конца соединения
    //    aut->db_reception(); //прием данных

    if (doc.object().value("protocol").toString() == "EnterTheWorld") //запрос на вход в мир
         world->newClient(clients[NS]); //добавляем клиента в мир (номер мира)

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

