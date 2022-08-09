#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    server = new Server;

    connect(server->aut, SIGNAL(addList(QString)), this, SLOT(addItem(QString))); //сигнал на добавление игрока
    connect(server, SIGNAL(DeleteClientList(int)), this, SLOT(RemoveList(int))); //сигнал на удаление игрока

    on_LaunchServer_clicked();
    updateList();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete server;
}


void MainWindow::updateList()
{
    int numberPlayrs = ui->listWidget->count();
    ui->QuanPlayers->setText(QString::number(numberPlayrs) + "/" + "100");
}


void MainWindow::addItem(QString item) //добавление строк
{
    ui->listWidget->addItem(item);
    updateList();
}


void MainWindow::RemoveList(int item) //удаление строк
{
    delete ui->listWidget->item(item);
    updateList();
}

void MainWindow::kickAll()
{
    for (int j = 0; j < server->clients.size(); j++) //отправка всем запросов о кике
        server->socketWrite("{\"Protocol\":\"kick\"}", server->clients[j]->avatar.socketDescriptor);
}

void MainWindow::on_LaunchServer_clicked() //запуск/отключение сервера
{
    if (server->isListening()) //отключение если активен
    {
        server->close(); //закрываем доступ к серверу
        ui->LaunchServer->setText("Запуск");

        ui->ActiveServer->setStyleSheet("color:red;");
        ui->ActiveServer->setText("Неактивен");

        kickAll();
    }
    else {
        bool active = server->LaunchServer(); //запускаем сервер
        if (active == true) {
            ui->LaunchServer->setText("Завершить");

            ui->ActiveServer->setStyleSheet("color:green;");
            ui->ActiveServer->setText("Активен");
        }
    }
}

void MainWindow::on_pushButton_clicked()
{ //кик выбранного игрока
    QByteArray itog;
    itog = "{\"Protocol\":\"kick\"}";
    server->socketWrite(itog, server->clients[ui->listWidget->currentRow()]->avatar.socketDescriptor);
}
