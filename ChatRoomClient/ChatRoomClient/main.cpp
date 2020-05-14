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
void MainWindow::on_pushButton_clicked()
{
    if (this->chat->recAU == 1) this->chat->recAU=0;
    else this->chat->recAU = 1;
}
