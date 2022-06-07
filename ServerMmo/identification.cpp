#include "identification.h"

identification::identification(Database *part_)
{
    part = part_;
}

void identification::identefication_(QJsonDocument doc, int socketDescriptor, QVector<Client*> pr)
{

    if (doc.object().value("type").toString() == "reg") { //запрос на регистрацию

        qDebug() << " Запрос на регистрацию";
        QString itog = "SELECT* FROM gamers WHERE password1 = \"" + doc.object().value("password").toString() + "\" "
                + "AND email = \"" + doc.object().value("gmail").toString() + "\"";

        part->query.exec(itog);
        part->query.first();

        if (part->query.value(1).toString() == "") //если ника нет такого то регистрировать
        {
            QString requer = "INSERT INTO gamers (name, password1, email) VALUES ";

            requer += "('" + doc.object().value("name").toString() + "','";
            requer += doc.object().value("password").toString() + "','";
            requer += doc.object().value("gmail").toString() + "')";

            if (part->query.exec(requer)) {

                emit socketWrite("{\"Write\":\"Registation thinks!\"}", socketDescriptor);
                emit addList(part->query.value("name").toString());


                for (int j = 0; j < pr.size(); j++) //сохраняем данные в клиенте
                    if (pr[j]->avatar.socketDescriptor == socketDescriptor)
                    {
                        pr[j]->idn.password = doc.object().value("password").toString();
                        pr[j]->idn.email =  doc.object().value("gmail").toString();
                    }
            }

        } else  //если есть
            emit socketWrite("{\"Write\":\"Users active\"}", socketDescriptor);

    }

    //---------------------------------------------

    if (doc.object().value("type").toString() == "aut") { //запрос на авторизацию

        qDebug() << "Запрос на авторизацию";

        //проверим не авторезирован ли пользователь на данный момент
        for (int c = 0; c < pr.size(); c++)
           if (pr[c]->idn.password == doc.object().value("password").toString() &&
               pr[c]->idn.email ==  doc.object().value("gmail").toString() &&
               pr[c]->avatar.socketDescriptor != socketDescriptor) {

               qDebug() << "Запрос отклонен - данный игрок онлайн";
               emit socketWrite("{\"Write\":\"Error:User Active\"}", socketDescriptor);
               return; //может еще подать запрос на удаление из коннекта
           }

        QString itog = "SELECT* FROM gamers WHERE password1 = '" + doc.object().value("password").toString() + "' "
                + "AND email = '" + doc.object().value("gmail").toString() + "'";

        part->query.exec(itog);
        part->query.first();

        if (part->query.value(3).toString() == doc.object().value("password").toString() &&
            part->query.value(4).toString() == doc.object().value("gmail").toString())
        {
            qDebug() << "Пользователь авторизирован";

            emit socketWrite("{\"Write\":\"User auntification\"}", socketDescriptor);
            emit addList(part->query.value(1).toString()); //добавляем в listwidget нового игрока


            for (int j = 0; j < pr.size(); j++) //сохраняем данные в клиенте
                if (pr[j]->avatar.socketDescriptor == socketDescriptor)
                {
                    pr[j]->idn.password = doc.object().value("password").toString();
                    pr[j]->idn.email =  doc.object().value("gmail").toString();
                }
        } else
            emit socketWrite("{\"Write\":\"Error auntification\"}", socketDescriptor);

    }
}
