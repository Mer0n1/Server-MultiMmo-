#ifndef AVATAR_H
#define AVATAR_H

#include <QMainWindow>
#include <QDialog>
#include "database.h"
#include <QTcpServer>
#include <QTcpSocket>

struct Avatar
{
    int id, pid;
    int socketDescriptor;
    int hp, maxHp;
    int x, y;
    bool life;
};


#endif // AVATAR_H
