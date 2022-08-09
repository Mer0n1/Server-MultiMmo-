#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <QMessageBox>
#include "database.h"
#include "identification.h"
#include "World.h"
#include "ServersMap.h" //test

class Server : public QTcpServer
{
    /* Класс отвечающий за принятие запросов клиентов их отправку
     * Также обьеднинение всех модулей
     */
    Q_OBJECT
public:
    Server();
    ~Server();

    QVector<Client*> clients; //клиенты
    QUdpSocket* UdpSocket;
    QByteArray Data; //куда складывать данные

    QJsonDocument doc;
    QJsonParseError docError;

    identification *aut;
    Database *part; //класс базы данных
    World *world; //Общий мир (хранит всех игроков. Мобов не хранит)

    QVector<Serv01Map*> maps; //активные сервер-карты
public slots:

    void incomingConnection(int socketDescriptor);//новое подключение
    bool LaunchServer(); //запуск сервера
    void DeleteClient(); //удаление клиента

    void sockReady(); //читать запросы TCP
    void sockUdpReady(); //UDP

    //слоты для сигналов
    void socketWrite(QByteArray str, int socketDescriptor); //передать данные сокету
    void socketStructWrite(Avatar*, Client*); //передать структуру данных сокету (класс аватар)

signals:
    void DeleteClientList(int);
};

#endif // SERVER_H
