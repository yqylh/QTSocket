#ifndef CHAT_H
#define CHAT_H
#include <QtNetwork>
#include <QTcpServer> //监听套接字
#include <QTcpSocket> //通信套接字//对方的(客户端的)套接字(通信套接字)
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QAbstractSocket>
#include <QByteArray>
#include <QString>
#include <vector>
#include <fstream>
#include <QMessageBox>
#include <QProcess>
#include <QFileDialog>
#include <QFileDialog>
class ChatServer : public QObject
{
    Q_OBJECT
private :
    QTcpServer *server;
    std::vector<QTcpSocket *> client;
    FILE *fp;
    int fileSend;
    MainWindow *window;
    std::map<int, int> q;
private slots:
    void acceptConnection() {
        qDebug() << "re";
        client.push_back(server->nextPendingConnection());
        connect(client.back(), SIGNAL(readyRead()), this,  SLOT(receiveMessage()));
        client.back()->write(("hello, your name is Client" + std::to_string(client.size()-1) + "\n").c_str());
        for(auto &i : client) {
            i->write(("Client" + std::to_string(client.size()-1) + "Join\n").c_str());
        }
        window->ui->ChatInfo->setPlainText(window->ui->ChatInfo->toPlainText()+"\nClient" + QString::fromStdString(std::to_string(client.size()-1)) + "Join");
    }
    void send(QTcpSocket * chat) {
        int len = 0;
        const int bufferSize = 1024;
        char *buffer;
        buffer = new char[bufferSize];  memset(buffer, 0, bufferSize);
        int num = 0;
        rewind(fp);
        while (len = fread(buffer, sizeof(char), bufferSize, fp)) {
            chat->write(buffer, len);
            memset(buffer, 0, bufferSize);
            qDebug() << num++;
        }
        chat->write("_%recived_%bye");
    }
    void receiveMessage() {
        for (int i = 0; i < client.size(); i++) {
            auto cli = client[i];
            QByteArray data = cli->read(1024);
            std::string stdStr = data.toStdString();
            QString str(data);
            if (stdStr == "begin") {
                qDebug() << "Begin Send";
                if (q[i] == 1) {
                    send(cli);
                    q.erase(i);
                }
                if (q.empty()) fclose(fp);
                return;
            }
            if (stdStr == "End") {
                if (q[i] == 1) {
                    q.erase(i);
                }
                if (q.empty()) fclose(fp);
                return;
            }

            if (fileSend==-1) {
                if (stdStr.find("_%recived_%") != -1) {
                    auto ans = QMessageBox::information(NULL, "info", "recived File, Whether to accept", QMessageBox::Yes, QMessageBox::No);
                    if (ans == QMessageBox::Yes) {
                        std::string name = stdStr.substr(stdStr.find("_%recived_%") + 11, stdStr.find("%_end")-11);
                        QString filePath = QFileDialog::getExistingDirectory(window, "Choose a folder to save", "./");
                        fp = fopen((filePath.toStdString()+"/" + name).c_str(), "wb");
                        cli->write("begin");
                        qDebug() << "first\n";
                        fileSend = i;
                    } else {
                        cli->write("End");
                    }
                } else if (data.size()!=0){
                    window->ui->ChatInfo->setPlainText(window->ui->ChatInfo->toPlainText()+"\n"+str);
                    for(auto &j : client) {
                        j->write(("Client" + std::to_string(i) + ": ").c_str() + data);
                        qDebug() << (std::to_string(i) + ": ").c_str();
                    }
                }
            } else {
                if (i != fileSend) {
                    cli->write("Sending File, Please wait");
                    continue;
                }
                qDebug() << "begin\n";
                int num = 0;
                while (data.size() != 0) {
                    if (data.toStdString().find("_%recived_%bye") != -1) {
                        std::string stdStr = data.toStdString();
                        data.remove(stdStr.find("_%recived_%bye"), data.length()-stdStr.find("_%recived_%bye"));
                        fwrite(data.data(), sizeof(char), data.length(), fp);
                        qDebug() << "end\n";
                        fclose(fp);
                        fileSend = -1;
                        break;
                    }
                    fwrite(data.data(), sizeof(char), data.length(), fp);
                    data=cli->read(1024);
                    qDebug() << num++;
                }
            }
        }
    }
public:
    friend class MainWindow;
    ChatServer(MainWindow *window) : window(window) {
        fileSend = -1;
        q.clear();
        fp = nullptr;
        client.clear();
        server = new QTcpServer();
        server->listen(QHostAddress::Any, 233);
        window->setWindowTitle("Server: 233");
        connect(server,SIGNAL(newConnection()), this, SLOT(acceptConnection()));
    };
    ~ChatServer() {
        server->close();
        for (auto & i : client) i->close();
    }
};

#endif // CHAT_H