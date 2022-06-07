#ifndef AVATAR_H
#define AVATAR_H

#include <QMainWindow>
#include <QDialog>
#include "database.h"
#include <QTcpServer>
#include <QTcpSocket>

class Avatar
{
    /* Класс-структура аватара игрока */
public:
    Avatar();

    unsigned int id;
    unsigned int p_id;
    unsigned int id_character;
    int socketDescriptor;

    int xp;
    unsigned int maxXp;

    int x, y;
    bool life; //жизнь
};


#endif // AVATAR_H
