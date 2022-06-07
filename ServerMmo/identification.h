#ifndef IDENTIFICATION_H
#define IDENTIFICATION_H

#include <QApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QFile>
#include <QSql>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QListWidgetItem>
#include "database.h"
#include "Client.h"

class identification : public QTcpServer
{
    Q_OBJECT
    /* Модуль индентефикации и обмена сохранений*/
public:
    identification(Database *part_);

public slots:

    //авторизация / регистрация
    void identefication_(QJsonDocument doc, int socketDescriptor, QVector<Client*> pr);
signals:
    void addList(QString); //сигнал для MainWindow
    void socketWrite(QByteArray, int); //сигнал для Server

private:
    Database *part;
};

#endif // IDENTIFICATION_H
