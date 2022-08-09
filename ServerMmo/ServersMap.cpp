#include "ServersMap.h"

Serv01Map::Serv01Map(Client* client, World* world_, QString nameMap_)
{
    ModuleSC = client;
    world = world_;
    nameMap = nameMap_;

    LoadWorld(nameMap, this);
    ModuleSC->socket->waitForConnected(500);
}

bool Serv01Map::CheckUdp(Avatar* av)
{
    if (av->socketDescriptor == 0) { //обработка протокола Клиент-Сервер

        if (mobs.size() == 0)
            mobs.push_back(*av); //добавление моба

        for (int j = 0; j < mobs.size(); j++) { //обновление данных моба
            if (mobs[j].id  == av->id &&
                mobs[j].pid == av->pid) {
                mobs[j] = *av;
                break;
            }
            else if(j == mobs.size() - 1)  //если такого нет добавляем
                mobs.push_back(*av);
        }

        //разошлем всем структуры данных мобов
        if (mobs.back().pid == av->pid &&
            mobs.back().id  == av->id) //отправляем после того как пришли все мобы
        for (int j = 0; j < player.size(); j++)
            if (player[j] != ModuleSC)
            {
                for (int n = 0; n < mobs.size(); n++)
                    world->socketStructWrite(&mobs[n], player[j]);
            }
        return true;
    }
    return false;
}

void Serv01Map::LoadWorld(QString nameMap, Serv01Map* map)
{
    QFile file("Maps/"+nameMap + ".xml");
    file.open(QFile::ReadOnly | QFile::Text);
    QXmlStreamReader xmlReader;
    xmlReader.setDevice(&file);
    xmlReader.readNext();
    xmlReader.readNext();
    xmlReader.readNext();
    xmlReader.readNext();

    QByteArray json;

    for (int j = 0; !xmlReader.atEnd(); j++) {

        if (j == 0)
            json = "{\"Protocol\":\"LoadServMap\",\"ptype\":\"begin\",";
        else
        if (j == xmlReader.atEnd() - 1)
            json = "{\"Protocol\":\"LoadServMap\",\"ptype\":\"end\",";
        else
            json = "{\"Protocol\":\"LoadServMap\",\"ptype\":\"current\",";

        if(xmlReader.isStartElement()) {
            if (xmlReader.name() == "character")
            {

                foreach(const QXmlStreamAttribute &attr, xmlReader.attributes())
                    if (attr.name().toString() == "type")
                        json += "\"type\":\"" + attr.value().toString() + "\",";

                foreach(const QXmlStreamAttribute &attr, xmlReader.attributes())
                    if (attr.name().toString() == "id")
                        json += "\"id\":\"" + attr.value().toString() + "\",";

                foreach(const QXmlStreamAttribute &attr, xmlReader.attributes())
                    if (attr.name().toString() == "IDO")
                        json += "\"IDO\":\"" + attr.value().toString() + "\",";

                foreach(const QXmlStreamAttribute &attr, xmlReader.attributes())
                    if (attr.name().toString() == "name")
                        json += "\"name\":\"" + attr.value().toString() + "\",";

                foreach(const QXmlStreamAttribute &attr, xmlReader.attributes())
                    if (attr.name().toString() == "x")
                        json += "\"x\":\"" + attr.value().toString() + "\",";

                foreach(const QXmlStreamAttribute &attr, xmlReader.attributes())
                    if (attr.name().toString() == "y")
                        json += "\"y\":\"" + attr.value().toString() + "\",";

                foreach(const QXmlStreamAttribute &attr, xmlReader.attributes())
                    if (attr.name().toString() == "hp")
                        json += "\"hp\":\"" + attr.value().toString() + "\",";

                foreach(const QXmlStreamAttribute &attr, xmlReader.attributes())
                    if (attr.name().toString() == "maxHp")
                        json += "\"maxHp\":\"" + attr.value().toString() + "\",";

                foreach(const QXmlStreamAttribute &attr, xmlReader.attributes())
                    if (attr.name().toString() == "speed")
                        json += "\"speed\":\"" + attr.value().toString() + "\",";

                foreach(const QXmlStreamAttribute &attr, xmlReader.attributes())
                    if (attr.name().toString() == "visibility")
                        json += "\"visibility\":\"" + attr.value().toString() + "\",";

                foreach(const QXmlStreamAttribute &attr, xmlReader.attributes())
                    if (attr.name().toString() == "DistanceFromOpponent")
                        json += "\"DistanceFromOpponent\":\"" + attr.value().toString() + "\",";

                foreach(const QXmlStreamAttribute &attr, xmlReader.attributes())
                    if (attr.name().toString() == "DistantionPoint")
                        json += "\"DistantionPoint\":\"" + attr.value().toString() + "\"";

                json += "}\n";
                map->json = json;
                emit world->socketWrite(json, ModuleSC->avatar.socketDescriptor);
                Sleep(50);
            }
        }
       xmlReader.readNext(); // Переходим к следующему элементу файла
    }
    file.close();
}
