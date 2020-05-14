#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow;
class ChatClient;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    friend class ChatClient;
    void initChat(ChatClient *chat) {
        this->chat = chat;
    }
private slots:

    void on_sendToServer_clicked();

    void on_sendFile_clicked();

private:
    Ui::MainWindow *ui;
    ChatClient *chat;
};

class ChatClient : public QObject{
    Q_OBJECT
private slots:
    void sendInfo();
    void receiveMessage();
private :
    MainWindow *window;
    friend class MainWindow;
    int ReciveFile;
public:
    QTcpSocket *client;
    ChatClient(MainWindow *window) : window(window) {
        ReciveFile = 0;
        client = new QTcpSocket();
        client->connectToHost(QHostAddress("127.0.0.1"), 233);
        connect(client, SIGNAL(readyRead()), this,  SLOT(receiveMessage()));
    };
    ~ChatClient() {
        client->close();
    }
};

#endif // MAINWINDOW_H
