#ifndef ENVIRONS_H
#define ENVIRONS_H

#include <QMainWindow>
#include "World.h"
#include <windows.h>

/*Файл .h является пробным - это всего лишь поиск правильного решения создания сихнронизации */
class Serv01Map : public World//, public QRunnable //конкретный сервер для обслуживания карты
{
    Q_OBJECT
   Client* ModuleSC; //module synchronization client
   World* world;
public:
   Serv01Map(Client* client, World* world_, QString name);
   void LoadWorld(QString nameMap, Serv01Map* map);
   bool CheckUdp(Avatar* mob);

   QByteArray json;
   QString nameMap;
};

//void LoadWorld(QString nameMap, Serv01Map* map); //загрузка мира

//пускай у будет 1 игрок, который будет передавать информацию о существах в мире и этот же
/* игрок будет являться сервером(специфичным, тоесть этот сервер-клиент)
 * Основной сервер в начале отправит ему карту и информацию о существах карты, затем сервер-клиент
 * будет отправлять эту информацию обратно. Возможно сервер-клиент - это не какой то
 * посторонний человек, а конкретный мой внешний модуль (тоесть клиент на моем компе)
 * */

#endif // ENVIRONS_H
