#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <fstream>
#include <QMessageBox>
#include <QProcess>
#include <QFileDialog>
#include <algorithm>
#include <QFileDialog>
#include <fstream>
#include <cstdio>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);    // 禁止最大化按钮
    setFixedSize(this->width(),this->height());                         // 禁止拖动窗口大小
    setWindowTitle("Client");
    ui->ChatInfo->setReadOnly(1);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void ChatClient::sendInfo(){
    client->write(window->ui->InputInfo->toPlainText().toStdString().c_str());
    window->ui->InputInfo->setPlainText("");
}
void MainWindow::on_sendToServer_clicked()
{
    this->chat->sendInfo();
}
bool sendFile = 0;
FILE *fp;
void send(ChatClient * chat) {
    int len = 0;
    const int bufferSize = 1024;
    char *buffer;
    buffer = new char[bufferSize];  memset(buffer, 0, bufferSize);
    int num = 0;
    while (len = fread(buffer, sizeof(char), bufferSize, fp)) {
        chat->client->write(buffer, len);
        memset(buffer, 0, bufferSize);
        qDebug() << num++;
    }
    chat->client->write("_%recived_%bye");
    fclose(fp);
    sendFile = 0;
}
void ChatClient::receiveMessage() {
    QByteArray data = client->readAll();
    if (sendFile) {
        if (data == "End") return;
        send(this);
        return;
    }
    if (ReciveFile == 1) {
        qDebug() << "begin\n";
        int num = 0;
        while (data.size() != 0) {
            if (data.toStdString().find("_%recived_%bye") != -1) {
                std::string stdStr = data.toStdString();
                data.remove(stdStr.find("_%recived_%bye"), data.length()-stdStr.find("_%recived_%bye"));
                fwrite(data.data(), sizeof(char), data.length(), fp);
                qDebug() << "end\n";
                fclose(fp);
                ReciveFile = -1;
                break;
            }
            fwrite(data.data(), sizeof(char), data.length(), fp);
            data=client->read(1024);
            qDebug() << num++;
        }
        return;
    }
    std::string stdStr = data.toStdString();
    QString str(data);
    if (stdStr.find("_%recived_%") != -1) {
        auto ans = QMessageBox::information(NULL, "info", "recived File, Whether to accept", QMessageBox::Yes, QMessageBox::No);
        if (ans == QMessageBox::Yes) {
            std::string name = stdStr.substr(stdStr.find("_%recived_%") + 11, stdStr.find("%_end")-11);
            QString filePath = QFileDialog::getExistingDirectory(window, "Choose a folder to save", "./");
            fp = fopen((filePath.toStdString()+"/" + name).c_str(), "wb");
            client->write("begin");
            qDebug() << "first\n";
            ReciveFile= 1;
        } else {
            client->write("End");
        }
        return;
    }
    if (data.size() != 0) {
        QString str(data);
        auto stdStr = str.toStdString();
        window->ui->ChatInfo->setPlainText(window->ui->ChatInfo->toPlainText()+"\n"+str);
    }
}
void MainWindow::on_sendFile_clicked()
{
    QString filePath = QFileDialog::getOpenFileName();
    std::string file = filePath.toStdString();
    std::string s, push, name;
    name = file.substr(file.rfind("/")+1, file.size());
   fp = fopen(file.c_str(), "rb");
   this->chat->client->write(("_%recived_%"+ name + "%_end").c_str(), ("_%recived_%"+ name + "%_end").length());
   sendFile = 1;
}
