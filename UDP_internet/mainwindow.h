#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QUdpSocket>
#include <QPlainTextEdit>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QUdpSocket* udpsocket;
    QHostAddress Multicast=QHostAddress::Broadcast; //监听群播地址的位置

protected slots:
    void do_readyRead(); //处理广播，单播，以及群播消息

public:
    //void read_Disp(QPlainTextEdit&edit,QByteArray array){edit.appendPlainText(array);}
    void read_prot();  //读取广播地址数据


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool eventFilter(QObject*,QEvent*)override;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_spinBox_2_editingFinished();


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
