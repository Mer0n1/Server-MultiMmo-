#include "World.h"


World::World()
{
    distance = 1000; //дистанция при которой происходит Replication()
}

World::~World()
{
    for (int j = 0; j < player.size(); j++)
        player.erase(player.begin()+j); //удаляем все аватары
}

void World::Replication(int nObj)
{

    for (int j = 0; j < player.size(); j++)
        if (nObj != j)
            if (player[nObj]->avatar.x - distance < player[j]->avatar.x && //использование радиуса(дистанции)
                player[nObj]->avatar.x + distance > player[j]->avatar.x &&
                player[nObj]->avatar.y - distance < player[j]->avatar.y &&
                player[nObj]->avatar.y + distance > player[j]->avatar.y) {
                emit socketStructWrite(&player[nObj]->avatar, player[j]);
            }
}


void World::newClient(Client* client) //добавление клиента в мир
{
    player.push_back(client); //создаем аватар для него
}

void World::DeleteClient(Client* client)
{
    for (int j = 0; j < player.size(); j++)
        if (player[j] == client) {
            player.remove(j);
            break;
        }
}

void World::Damage(unsigned int p_id, int damage)
{
    for (int j = 0; j < player.size(); j++) //ищем нужный подайди
        if (player[j]->avatar.pid == p_id) {
            player[j]->avatar.hp -= damage; //наносим урон
            emit socketStructWrite(&player[j]->avatar, player[j]); //отправляем структуру данных с изменениями этому же клиенту
            break;
        }
}
