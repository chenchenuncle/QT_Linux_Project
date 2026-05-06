#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QMainWindow>
#include <QSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QSerialPort* port;//串口端口
    QList<QLabel*> labels;//标签点击按钮

public slots:
    void do_errorOccurred(QSerialPort::SerialPortError error);
    void do_readyRead();//

public:
    virtual bool eventFilter(QObject *watched, QEvent *event);
    QSerialPort::StopBits stopbits(float);
    void ion();//修改标签按键事件

    //测试功能
    void Test() {
        ui->textEdit->append("名字:"+port->portName());
        ui->textEdit->append("波特率::"+QString("%1").arg(port->baudRate()));
        ui->textEdit->append("数据位:"+QString("%1").arg(port->dataBits()));
        ui->textEdit->append("停止位:"+QString("%1").arg(port->stopBits()));
        ui->textEdit->append("校验位:"+QString("%1").arg(port->parity()));
        ui->textEdit->append(port->errorString());
    }

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_action_1_triggered();

    void on_action_2_triggered();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
