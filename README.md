__Сервер созданный специально для игры MultiMmo.__

Сервер имеет модули
* Идентификации
* Игрового мира/карты

А также способен принимать и отправлять запросы udp со структурами данных между игроками игрового мира/карты.

__Внимание:__ сервер не будет работать без базы данных sql. Для этого необходимо иметь PhpMyAdmin (я использую MAMP). В ней нужно создать базу данных rpg и таблицу gamers со столбцами id, name, login, password1, email, а также заполнить ее. 