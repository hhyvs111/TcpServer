/********************************************************************************
** Form generated from reading UI file 'TcpServer.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TCPSERVER_H
#define UI_TCPSERVER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TcpServer
{
public:
    QWidget *centralwidget;
    QPushButton *startBtn;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *TcpServer)
    {
        if (TcpServer->objectName().isEmpty())
            TcpServer->setObjectName(QStringLiteral("TcpServer"));
        TcpServer->resize(257, 331);
        centralwidget = new QWidget(TcpServer);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        startBtn = new QPushButton(centralwidget);
        startBtn->setObjectName(QStringLiteral("startBtn"));
        startBtn->setGeometry(QRect(90, 120, 99, 27));
        TcpServer->setCentralWidget(centralwidget);
        menubar = new QMenuBar(TcpServer);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 257, 28));
        TcpServer->setMenuBar(menubar);
        statusbar = new QStatusBar(TcpServer);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        TcpServer->setStatusBar(statusbar);

        retranslateUi(TcpServer);

        QMetaObject::connectSlotsByName(TcpServer);
    } // setupUi

    void retranslateUi(QMainWindow *TcpServer)
    {
        TcpServer->setWindowTitle(QApplication::translate("TcpServer", "MainWindow", Q_NULLPTR));
        startBtn->setText(QApplication::translate("TcpServer", "\345\274\200\345\220\257\346\234\215\345\212\241\345\231\250", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class TcpServer: public Ui_TcpServer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TCPSERVER_H
