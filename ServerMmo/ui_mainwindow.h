/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTextBrowser *textBrowser;
    QLabel *ActiveServer;
    QLabel *QuanPlayers;
    QListWidget *listWidget;
    QPushButton *LaunchServer;
    QLabel *NameMap;
    QPushButton *pushButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(457, 359);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        textBrowser = new QTextBrowser(centralwidget);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(10, 10, 171, 61));
        ActiveServer = new QLabel(centralwidget);
        ActiveServer->setObjectName(QString::fromUtf8("ActiveServer"));
        ActiveServer->setGeometry(QRect(20, 20, 71, 16));
        QuanPlayers = new QLabel(centralwidget);
        QuanPlayers->setObjectName(QString::fromUtf8("QuanPlayers"));
        QuanPlayers->setGeometry(QRect(20, 40, 51, 21));
        listWidget = new QListWidget(centralwidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(200, 10, 231, 271));
        LaunchServer = new QPushButton(centralwidget);
        LaunchServer->setObjectName(QString::fromUtf8("LaunchServer"));
        LaunchServer->setGeometry(QRect(100, 40, 75, 23));
        NameMap = new QLabel(centralwidget);
        NameMap->setObjectName(QString::fromUtf8("NameMap"));
        NameMap->setGeometry(QRect(100, 10, 71, 31));
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(360, 290, 75, 23));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 457, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        ActiveServer->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" color:#ff0000;\">\320\235\320\265\320\260\320\272\321\202\320\270\320\262\320\265\320\275</span></p></body></html>", nullptr));
        QuanPlayers->setText(QCoreApplication::translate("MainWindow", "0/0", nullptr));
        LaunchServer->setText(QCoreApplication::translate("MainWindow", "\320\227\320\260\320\277\321\203\321\201\320\272", nullptr));
        NameMap->setText(QCoreApplication::translate("MainWindow", "\320\232\320\260\321\200\321\202\320\260", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Kick", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
