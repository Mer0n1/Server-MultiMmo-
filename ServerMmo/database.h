#ifndef DATABASE_H
#define DATABASE_H

#include <QSql>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>

struct Database
{
    QSqlDatabase *db; //база данных
    QSqlQuery query;
};

#endif // DATABASE_H
