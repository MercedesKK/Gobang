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
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *regret;
    QLabel *gameStatus;
    QLabel *who;
    QPushButton *restart;
    QPushButton *PVP;
    QPushButton *PVE;
    QLabel *label;
    QLabel *label_2;
    QLabel *time;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(814, 734);
        MainWindow->setFocusPolicy(Qt::NoFocus);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        regret = new QPushButton(centralwidget);
        regret->setObjectName(QString::fromUtf8("regret"));
        regret->setGeometry(QRect(650, 310, 111, 61));
        regret->setFocusPolicy(Qt::NoFocus);
        gameStatus = new QLabel(centralwidget);
        gameStatus->setObjectName(QString::fromUtf8("gameStatus"));
        gameStatus->setGeometry(QRect(660, 40, 101, 31));
        who = new QLabel(centralwidget);
        who->setObjectName(QString::fromUtf8("who"));
        who->setGeometry(QRect(660, 100, 101, 31));
        restart = new QPushButton(centralwidget);
        restart->setObjectName(QString::fromUtf8("restart"));
        restart->setGeometry(QRect(650, 220, 111, 61));
        restart->setFocusPolicy(Qt::NoFocus);
        PVP = new QPushButton(centralwidget);
        PVP->setObjectName(QString::fromUtf8("PVP"));
        PVP->setGeometry(QRect(650, 520, 111, 61));
        PVP->setFocusPolicy(Qt::NoFocus);
        PVE = new QPushButton(centralwidget);
        PVE->setObjectName(QString::fromUtf8("PVE"));
        PVE->setGeometry(QRect(650, 610, 111, 61));
        PVE->setFocusPolicy(Qt::NoFocus);
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(660, 380, 131, 141));
        label->setPixmap(QPixmap(QString::fromUtf8(":/background/Picture/tongji.jpg")));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 660, 201, 31));
        time = new QLabel(centralwidget);
        time->setObjectName(QString::fromUtf8("time"));
        time->setGeometry(QRect(640, 160, 141, 41));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 814, 26));
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
        regret->setText(QCoreApplication::translate("MainWindow", "\346\202\224\346\243\213", nullptr));
        gameStatus->setText(QCoreApplication::translate("MainWindow", "\346\243\213\345\261\200\350\277\233\350\241\214\344\270\255", nullptr));
        who->setText(QCoreApplication::translate("MainWindow", "\350\257\267\347\231\275\346\243\213\350\220\275\345\255\220", nullptr));
        restart->setText(QCoreApplication::translate("MainWindow", "\351\207\215\346\226\260\345\274\200\345\261\200", nullptr));
        PVP->setText(QCoreApplication::translate("MainWindow", "PVP", nullptr));
        PVE->setText(QCoreApplication::translate("MainWindow", "PVE", nullptr));
        label->setText(QString());
        label_2->setText(QCoreApplication::translate("MainWindow", "\345\274\200\345\217\221\344\272\272\345\221\230\357\274\232\345\274\240\351\235\226\345\207\257\343\200\201\346\227\266\345\244\251\351\200\270", nullptr));
        time->setText(QCoreApplication::translate("MainWindow", "AI\346\211\247\350\241\214\346\227\266\351\227\264\357\274\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
