#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QAudioFormat>
#include <QAudioInput>
#include <QIODevice>
#include <QAudioOutput>
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

struct videoPack{
    char    data[1024];
    int     lens;
};

class ChatClient : public QObject{
    Q_OBJECT
private slots:
    void sendInfo();
    void receiveMessage();
    void readyReadSlot() {
        qDebug() << "video reveiver...";
        videoPack vp;
        memset (&vp, 0, sizeof(vp));
        videoReadSocket->readDatagram ((char*)&vp, sizeof(vp));
        outputDevice->write (vp.data, vp.lens);
    }
private :
    MainWindow *window;
    friend class MainWindow;
    int ReciveFile;
    QUdpSocket *videoReadSocket;
    QAudioOutput *output;
    QIODevice *outputDevice;
public:
    QTcpSocket *client;
    ChatClient(MainWindow *window) : window(window) {
        ReciveFile = 0;
        client = new QTcpSocket();
        client->connectToHost(QHostAddress("127.0.0.1"), 233);
        connect(client, SIGNAL(readyRead()), this,  SLOT(receiveMessage()));

        videoReadSocket = new QUdpSocket(this);
        //绑定到广播端口
        videoReadSocket->bind (QHostAddress::AnyIPv4, 234,
                               QUdpSocket::ShareAddress
                               | QUdpSocket::ReuseAddressHint);
        //加入广播组
        videoReadSocket->joinMulticastGroup (QHostAddress("127.0.0.1"));
        //如果有套接字动作将会触发readyRead信号，我们将读取音频信息
        connect(videoReadSocket, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));
        //设置声音格式
        QAudioFormat format;
        format.setSampleRate (8000);        //设置采样频率
        format.setChannelCount (1);         //设置通道计数
        format.setSampleSize (16);          //设置样本大小，一般为8或者16
        format.setCodec ("audio/pcm");      //设置编码格式
        format.setSampleType (QAudioFormat::SignedInt);   //设置采样类型
        format.setByteOrder (QAudioFormat::LittleEndian); //设置字节序为小端字节序
        output = new QAudioOutput(format, this);

        //开启读取音频数据的接口
        outputDevice = output->start ();
    };
    ~ChatClient() {
        client->close();
    }
};

#endif // MAINWINDOW_H
