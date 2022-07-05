#ifndef WORLD_H
#define WORLD_H

#include <QMainWindow>
#include <QDialog>
#include "Avatar.h"
#include "Client.h"

class World : public QObject
{
    Q_OBJECT
    /*
     * Модуль отвечающий за мир и взаимодействие между игроками
     *
     * */
public:
    World();
    ~World();

    QVector<Client*> player; //игроки находящиеся в данном мире

    void Replication(int); //обновление окружающего мира вокруг игроков
    void Damage(unsigned int p_id, int damage); //нанесение урона игроку

    void newClient(Client*); //добавить нового игрока в мир
    void DeleteClient(Client*); //обновить список игроков (удаляет неактивных)

signals:
    void socketStructWrite(Avatar*, Client*);
private:
    int distance; //дистанция для репликации окружающего мира
};

#endif // WORLD_H
