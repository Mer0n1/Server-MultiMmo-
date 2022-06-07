#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpServer>
#include <QTcpSocket>
#include "Avatar.h"

struct IdentificationData
{ //файлы идентефикации (все файлы по которым пользователя распознают)
    QString email;
    QString password;
    QString login;
    QString name;
    int id;
};

struct Client
{
    QHostAddress sender;
    quint16 senderPort;
    Avatar avatar; //персонаж
    IdentificationData idn; //данные об аккаунте
    QTcpSocket* socket;
};

#endif // CLIENT_H
