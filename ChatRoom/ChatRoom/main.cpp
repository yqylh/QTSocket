#include "mainwindow.h"
#include <QApplication>
#include "chat.h"
#include <QFileDialog>
#include <fstream>
#include <map>
ChatServer *server;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    server = new ChatServer(&w);
    return a.exec();
}
void MainWindow::on_sendToClient_clicked()
{
    for(auto &i : server->client) {
        i->write(server->window->ui->InputInfo->toPlainText().toStdString().c_str());
    }
    server->window->ui->InputInfo->setPlainText("");
}
void MainWindow::on_sendFile_clicked()
{
   QString filePath = QFileDialog::getOpenFileName();
   QTextCodec *code = QTextCodec::codecForName("GB2312");
   std::string file = code->fromUnicode(filePath).data();
//   std::string file = filePath.toStdString();
   std::string s, push, name;
   name = file.substr(file.rfind("/")+1, file.size());
   server->fp = fopen(file.c_str(), "rb");
   for (auto &i : server->client) i->write(("_%recived_%"+ name + "%_end").c_str(), ("_%recived_%"+ name + "%_end").length());
   for (int i = 0; i < server->client.size(); i++) server->q[i]=1;
}

void MainWindow::on_pushButton_clicked()
{
    if (server->sendAu == 1) {
        server->sendAu = 0;
    } else {
        server->sendAu = 1;
    }
}
