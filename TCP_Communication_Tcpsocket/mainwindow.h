#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QLabel>
#include <QUuid>
#include <QHostAddress>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QTcpSocket* socket;//客户端
    QUuid ID;//ID
    QLabel* labeID;//
    QLabel* labestcket;//
    QHostAddress ip; //
    qint16 ports;

public slots:
    void do_connected();//建立连接时使用
    void do_errorOccurred();//当有错误信息时
    void do_readyRead();  //当被发送信息时

public:
    MainWindow(QString ip,qint16 port,QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_action_1_triggered();

    void on_action_2_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
