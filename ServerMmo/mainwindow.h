#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Server.h"
#include <iostream>
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Server *server;

public slots:
    void updateList(); //обновить список игроков
    void addItem(QString); //добавить в список нового клиента
    void RemoveList(int); //удалить из списка клиента

    void on_LaunchServer_clicked(); //запуск и остановка сервера
    void on_pushButton_clicked(); //kicked

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
