#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qlabel.h"
#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUuid>
#include <QHash>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QHash<QUuid,QTcpSocket*> temporaryID;//临时连接客户端ID
    QTcpServer* tcpserver;//建立服务器
    QTcpSocket* ID;//连接服务器
    QUuid uuid_ID;
    QLabel* label ;       //显示监听状态
    QLabel* port;         //显示端口

protected slots:
    void do_connected(){uuid_ID=QUuid::createUuid();} //获取临时ID给客户端
    void do_disconnected();//客户端要端开服务端时
    void do_readyRead();//客户端向客户端发起信息时
    void do_newConnection();//每当有新客户段连接时
    void do_acceptError(QAbstractSocket::SocketError socketError);//连接客户端失败时
    void do_pendingConnectionAvailable();//成功加入服务段时

public:
    QUuid Handle(QByteArray&);//处理分割信号并且隔离ID和数据流
    void Client_ID(QUuid,QByteArray&);//向客户端发送数据
    void Error(QTcpSocket*,QString); //向发送者提供发送信息出现错误信号

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_action_1_triggered();

    void on_action_2_triggered();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
