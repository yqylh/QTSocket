#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    ChatClient server(&w);
    w.initChat(&server);
    return a.exec();
}
