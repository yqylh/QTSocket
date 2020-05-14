/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *sendToClient;
    QPlainTextEdit *ChatInfo;
    QPlainTextEdit *InputInfo;
    QPushButton *sendFile;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(469, 447);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        sendToClient = new QPushButton(centralwidget);
        sendToClient->setObjectName(QString::fromUtf8("sendToClient"));
        sendToClient->setGeometry(QRect(220, 380, 161, 71));
        ChatInfo = new QPlainTextEdit(centralwidget);
        ChatInfo->setObjectName(QString::fromUtf8("ChatInfo"));
        ChatInfo->setGeometry(QRect(0, 0, 381, 381));
        InputInfo = new QPlainTextEdit(centralwidget);
        InputInfo->setObjectName(QString::fromUtf8("InputInfo"));
        InputInfo->setGeometry(QRect(0, 380, 221, 71));
        sendFile = new QPushButton(centralwidget);
        sendFile->setObjectName(QString::fromUtf8("sendFile"));
        sendFile->setGeometry(QRect(380, 0, 91, 71));
        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        sendToClient->setText(QCoreApplication::translate("MainWindow", "\345\217\221\351\200\201", nullptr));
        sendFile->setText(QCoreApplication::translate("MainWindow", "\345\217\221\351\200\201\346\226\207\344\273\266", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
