#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHostAddress>
#include <QNetworkInterface>
#include <QMessageBox>
#include <QByteArray>
#include <QThread>

void MainWindow::do_connected()
{
    ui->plainTextEdit->appendPlainText("连接成功");
}

void MainWindow::do_errorOccurred()
{
    QMessageBox::information(this,"出现错误,联系开发者",socket->errorString()+QString("%1").arg(socket->state()));
}

void MainWindow::do_readyRead()
{
    QByteArray array=socket->readAll();

    //获取ID
    if(ID.isNull())
    {
        ID=QUuid::fromString(array);
        labeID->setText("临时ID:"+ID.toString(QUuid::WithoutBraces));
    }
    else
    {
        ui->plainTextEdit->appendPlainText(array);
    }
}

MainWindow::MainWindow(QString IP, qint16 port, QWidget *parent)
    :QMainWindow(parent),ip(IP),ports(port)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //获取数据模块
    socket=new QTcpSocket(this);
    labeID=new QLabel(this);
    labestcket=new QLabel(this);


    /*显示本地IPv4地址
    const QList<QHostAddress> listinfo=QNetworkInterface::allAddresses();
    for(const auto &i:listinfo)
        if(i.isGlobal()&&i.protocol()==QAbstractSocket::IPv4Protocol)
        {
            this->setWindowTitle(i.toString());
        }*/

    /*
    if(socket->state()==QAbstractSocket::UnconnectedState)
        QMessageBox::information(this,"绑定问题地址",socket->errorString());*/

    //显示ID和状态栏
    ui->statusbar->addWidget(labeID,250);
    ui->statusbar->addPermanentWidget(labestcket,250);
    labestcket->setText("为连接服务器");
    labestcket->setAlignment(Qt::AlignRight);

    //建立读取信号
    connect(socket,&QTcpSocket::connected,this,&MainWindow::do_connected);
    connect(socket,&QTcpSocket::errorOccurred,this,&MainWindow::do_errorOccurred);
    connect(socket,&QTcpSocket::readyRead,this,&MainWindow::do_readyRead);

    //自动连接
    on_action_1_triggered();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if(ui->lineEdit->text().isEmpty()||ui->lineEdit_2->text().isEmpty())
    {
        QMessageBox::information(this,"提示","无ID或信息");
        return;
    }

    //获取发送消息和客户端ID
    QByteArray array=ui->lineEdit->text().toUtf8()+':'+ui->lineEdit_2->text().toUtf8();

    //发送消息成功
    if(socket->write(array)!=-1)
    {
        ui->plainTextEdit->appendPlainText("我:"+ui->lineEdit_2->text());
        ui->lineEdit_2->clear();
    }
    else
    {
        QMessageBox::information(this,"发送信息失败原因",socket->errorString());
    }
}


void MainWindow::on_action_1_triggered()
{
    //尝试连接服务器
    socket->connectToHost(ip,ports);

    ui->action_1->setEnabled(false);
    ui->action_2->setEnabled(true);
    ui->pushButton->setEnabled(true);
}

void MainWindow::on_action_2_triggered()
{
    //关闭服务器连接
    socket->disconnectFromHost();

    ui->action_1->setEnabled(true);
    ui->action_2->setEnabled(false);
    ui->pushButton->setEnabled(false);

    //清空ID
    ID=QUuid();
    labeID->clear();
}

