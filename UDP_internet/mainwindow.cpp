#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QNetworkInterface>
#include <QHostAddress>
#include <QByteArray>
#include <QNetworkDatagram>
#include <QMessageBox>

void MainWindow::do_readyRead()//处理广播，单播，组播的消息
{
    //处理数据流丢失可能
    if(udpsocket->pendingDatagramSize()==-1)
    {
        ui->plainTextEdit->appendPlainText("读取数据失败原因："+udpsocket->errorString());
        return;
    }
    QNetworkDatagram udpcin=udpsocket->receiveDatagram();
    if(!udpcin.isValid())
    {
        ui->plainTextEdit->appendPlainText("数据丢失");
        return;
    }

    //获取数据流
    QHostAddress ipcout=udpcin.senderAddress();
    QByteArray array=ipcout.toString().toUtf8()+':'+udpcin.data();

    //对各种播法的数据流
    QHostAddress ip=udpcin.destinationAddress();
    if(ip==QHostAddress::Broadcast)//广播
        ui->plainTextEdit->appendPlainText("(广播)"+array);
    else if(ip.isMulticast())//组播消息
        ui->plainTextEdit_2->appendPlainText(array);
    else //单播
        ui->plainTextEdit->appendPlainText(array);

}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //
    udpsocket=new QUdpSocket(this);

    /*获取IPv4
    QList<QHostAddress> listIPv4=QNetworkInterface::allAddresses();
    for(const auto &i:listIPv4)
        if(i.isGlobal()&&i.protocol()==QAbstractSocket::IPv4Protocol)
        {
            if(!udpsocket->bind(i)) ui->plainTextEdit->appendPlainText("连接IP错误："+udpsocket->errorString());
            this->setWindowTitle(i.toString());
            break;
        }*/
    if(!udpsocket->bind(QHostAddress::AnyIPv4)) ui->plainTextEdit->appendPlainText("连接IP错误:"+udpsocket->errorString());
    this->setWindowTitle(udpsocket->localAddress().toString());

    //显示绑定端口
    ui->spinBox_2->blockSignals(true);
    ui->spinBox_2->setValue(udpsocket->localPort());
    ui->spinBox_2->blockSignals(false);

    //获取组播地址输入栏全部事件
    ui->comboBox_2->installEventFilter(this);
    //this->eventFilter(ui->comboBox_2,QEvent*event);

    //建立读取信号
    connect(udpsocket,&QUdpSocket::readyRead,this,&MainWindow::do_readyRead);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if(object!=ui->comboBox_2)
        return QMainWindow::eventFilter(object,event);
    if(event->type()==QEvent::FocusOut)
    {
        //获取新的组播地址是否正确
        if(Multicast.toString()==ui->comboBox_2->currentText())
            return QMainWindow::eventFilter(object,event);
        if(QHostAddress(ui->comboBox_2->currentText()).isNull())
        {

            QMessageBox::information(this,"组播地址错误",udpsocket->errorString());
            return QMainWindow::eventFilter(object,event);
        }

        //重新加入组播
        udpsocket->leaveMulticastGroup(Multicast);
        Multicast.setAddress(ui->comboBox_2->currentText());
        if(!udpsocket->joinMulticastGroup(Multicast))
        {
            QMessageBox::information(this,"加入组播失败原因：",udpsocket->errorString());
        }
    }
    return QMainWindow::eventFilter(object,event);
}

void MainWindow::on_pushButton_clicked()
{
    //获取当前端口和对方地址
    QHostAddress ip(ui->comboBox->currentText());
    int prot=ui->spinBox->value();
    if(!prot||ip.isNull())
    {
        ui->plainTextEdit->appendPlainText("IP或者端口错误");
        return;
    }

    //获取自己ID和数据流
    QByteArray array=ui->lineEdit->text().toUtf8();

    //打包数据发送给对方
    QNetworkDatagram udpcout(array,ip,prot);
    if(udpsocket->writeDatagram(udpcout)==-1)
    {
        QMessageBox::information(this,"发送失败原因",udpsocket->errorString());
        return;
    }

    //显示发送成功后的消息
    ui->plainTextEdit->appendPlainText("我："+ui->lineEdit->text());
    ui->lineEdit->clear();
}


void MainWindow::on_pushButton_3_clicked()
{
    //获取当前端口和自己地址
    QHostAddress ip(ui->comboBox->currentText());
    int prot=ui->spinBox->value();
    if(!prot||ip.isNull())
    {
        ui->plainTextEdit->appendPlainText("IP或者端口错误");
        return;
    }

    //获取自己ID和数据流
    QByteArray array=ui->lineEdit->text().toUtf8();

    //打包数据发送给广播地址
    QNetworkDatagram udpcout(array,QHostAddress::Broadcast,prot);
    if(udpsocket->writeDatagram(udpcout)==-1)
    {
        QMessageBox::information(this,"发送失败原因",udpsocket->errorString());
    }

    //显示成功发送的消息
    ui->plainTextEdit->appendPlainText("我:"+ui->lineEdit->text());
    ui->lineEdit->clear();
}

void MainWindow::on_pushButton_2_clicked()
{
    //打包数据流,个人信息
    QNetworkDatagram data;
    data.setDestination(Multicast,ui->spinBox->value());
    data.setData(ui->lineEdit_2->text().toUtf8());

    //监测发送数据流是否成功
    if(udpsocket->writeDatagram(data)==-1)
    {
        QMessageBox::information(this,"发送组播消息失败",udpsocket->errorString());
        return;
    }

    //发送成功显示消息栏
    ui->plainTextEdit_2->appendPlainText("我"+ui->lineEdit_2->text());
    ui->lineEdit_2->clear();
}



void MainWindow::on_spinBox_2_editingFinished()
{
    if(ui->spinBox->value()==udpsocket->localPort())
        return;

    //重新开启套接字连接
    if(udpsocket->state()==QAbstractSocket::BoundState)udpsocket->close();
    if(!udpsocket->bind(QHostAddress::AnyIPv4,ui->spinBox->value()))
    {
        ui->plainTextEdit->appendPlainText("端口被占用,随机获取接口"+udpsocket->errorString());
    }

    //自动获取端口
    if(!udpsocket->isValid())udpsocket->bind(QHostAddress(this->windowTitle()));

    //避免再调用一次
    ui->spinBox_2->blockSignals(true);
    ui->spinBox_2->setValue(udpsocket->localPort());
    ui->spinBox_2->blockSignals(false);
}


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QNetworkInterface>
#include <QHostAddress>
#include <QByteArray>
#include <QNetworkDatagram>
#include <QMessageBox>

void MainWindow::do_readyRead()//处理广播，单播，组播的消息
{
    //处理数据流丢失可能
    if(udpsocket->pendingDatagramSize()==-1)
    {
        ui->plainTextEdit->appendPlainText("读取数据失败原因："+udpsocket->errorString());
        return;
    }
    QNetworkDatagram udpcin=udpsocket->receiveDatagram();
    if(!udpcin.isValid())
    {
        ui->plainTextEdit->appendPlainText("数据丢失");
        return;
    }

    //获取数据流
    QHostAddress ipcout=udpcin.senderAddress();
    QByteArray array=ipcout.toString().toUtf8()+':'+udpcin.data();

    //对各种播法的数据流
    QHostAddress ip=udpcin.destinationAddress();
    if(ip==QHostAddress::Broadcast)//广播
        ui->plainTextEdit->appendPlainText("(广播)"+array);
    else if(ip.isMulticast())//组播消息
        ui->plainTextEdit_2->appendPlainText(array);
    else //单播
        ui->plainTextEdit->appendPlainText(array);

}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //
    udpsocket=new QUdpSocket(this);

    /*获取IPv4
    QList<QHostAddress> listIPv4=QNetworkInterface::allAddresses();
    for(const auto &i:listIPv4)
        if(i.isGlobal()&&i.protocol()==QAbstractSocket::IPv4Protocol)
        {
            if(!udpsocket->bind(i)) ui->plainTextEdit->appendPlainText("连接IP错误："+udpsocket->errorString());
            this->setWindowTitle(i.toString());
            break;
        }*/
    if(!udpsocket->bind(QHostAddress::AnyIPv4)) ui->plainTextEdit->appendPlainText("连接IP错误:"+udpsocket->errorString());
    this->setWindowTitle(udpsocket->localAddress().toString());

    //显示绑定端口
    ui->spinBox_2->blockSignals(true);
    ui->spinBox_2->setValue(udpsocket->localPort());
    ui->spinBox_2->blockSignals(false);

    //获取组播地址输入栏全部事件
    ui->comboBox_2->installEventFilter(this);
    //this->eventFilter(ui->comboBox_2,QEvent*event);

    //建立读取信号
    connect(udpsocket,&QUdpSocket::readyRead,this,&MainWindow::do_readyRead);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if(object!=ui->comboBox_2)
        return QMainWindow::eventFilter(object,event);
    if(event->type()==QEvent::FocusOut)
    {
        //获取新的组播地址是否正确
        if(Multicast.toString()==ui->comboBox_2->currentText())
            return QMainWindow::eventFilter(object,event);
        if(QHostAddress(ui->comboBox_2->currentText()).isNull())
        {

            QMessageBox::information(this,"组播地址错误",udpsocket->errorString());
            return QMainWindow::eventFilter(object,event);
        }

        //重新加入组播
        udpsocket->leaveMulticastGroup(Multicast);
        Multicast.setAddress(ui->comboBox_2->currentText());
        if(!udpsocket->joinMulticastGroup(Multicast))
        {
            QMessageBox::information(this,"加入组播失败原因：",udpsocket->errorString());
        }
    }
    return QMainWindow::eventFilter(object,event);
}

void MainWindow::on_pushButton_clicked()
{
    //获取当前端口和对方地址
    QHostAddress ip(ui->comboBox->currentText());
    int prot=ui->spinBox->value();
    if(!prot||ip.isNull())
    {
        ui->plainTextEdit->appendPlainText("IP或者端口错误");
        return;
    }

    //获取自己ID和数据流
    QByteArray array=ui->lineEdit->text().toUtf8();

    //打包数据发送给对方
    QNetworkDatagram udpcout(array,ip,prot);
    if(udpsocket->writeDatagram(udpcout)==-1)
    {
        QMessageBox::information(this,"发送失败原因",udpsocket->errorString());
        return;
    }

    //显示发送成功后的消息
    ui->plainTextEdit->appendPlainText("我："+ui->lineEdit->text());
    ui->lineEdit->clear();
}


void MainWindow::on_pushButton_3_clicked()
{
    //获取当前端口和自己地址
    QHostAddress ip(ui->comboBox->currentText());
    int prot=ui->spinBox->value();
    if(!prot||ip.isNull())
    {
        ui->plainTextEdit->appendPlainText("IP或者端口错误");
        return;
    }

    //获取自己ID和数据流
    QByteArray array=ui->lineEdit->text().toUtf8();

    //打包数据发送给广播地址
    QNetworkDatagram udpcout(array,QHostAddress::Broadcast,prot);
    if(udpsocket->writeDatagram(udpcout)==-1)
    {
        QMessageBox::information(this,"发送失败原因",udpsocket->errorString());
    }

    //显示成功发送的消息
    ui->plainTextEdit->appendPlainText("我:"+ui->lineEdit->text());
    ui->lineEdit->clear();
}

void MainWindow::on_pushButton_2_clicked()
{
    //打包数据流,个人信息
    QNetworkDatagram data;
    data.setDestination(Multicast,ui->spinBox->value());
    data.setData(ui->lineEdit_2->text().toUtf8());

    //监测发送数据流是否成功
    if(udpsocket->writeDatagram(data)==-1)
    {
        QMessageBox::information(this,"发送组播消息失败",udpsocket->errorString());
        return;
    }

    //发送成功显示消息栏
    ui->plainTextEdit_2->appendPlainText("我"+ui->lineEdit_2->text());
    ui->lineEdit_2->clear();
}



void MainWindow::on_spinBox_2_editingFinished()
{
    if(ui->spinBox->value()==udpsocket->localPort())
        return;

    //重新开启套接字连接
    if(udpsocket->state()==QAbstractSocket::BoundState)udpsocket->close();
    if(!udpsocket->bind(QHostAddress::AnyIPv4,ui->spinBox->value()))
    {
        ui->plainTextEdit->appendPlainText("端口被占用,随机获取接口"+udpsocket->errorString());
    }

    //自动获取端口
    if(!udpsocket->isValid())udpsocket->bind(QHostAddress(this->windowTitle()));

    //避免再调用一次
    ui->spinBox_2->blockSignals(true);
    ui->spinBox_2->setValue(udpsocket->localPort());
    ui->spinBox_2->blockSignals(false);
}



void MainWindow::do_readyRead()//处理广播，单播，组播的消息
{
    //处理数据流丢失可能
    if(udpsocket->pendingDatagramSize()==-1)
    {
        ui->plainTextEdit->appendPlainText("读取数据失败原因："+udpsocket->errorString());
        return;
    }
    QNetworkDatagram udpcin=udpsocket->receiveDatagram();
    if(!udpcin.isValid())
    {
        ui->plainTextEdit->appendPlainText("数据丢失");
        return;
    }

    //获取数据流
    QHostAddress ipcout=udpcin.senderAddress();
    QByteArray array=ipcout.toString().toUtf8()+':'+udpcin.data();

    //对各种播法的数据流
    QHostAddress ip=udpcin.destinationAddress();
    if(ip==QHostAddress::Broadcast)//广播
        ui->plainTextEdit->appendPlainText("(广播)"+array);
    else if(ip.toString()==this->windowTitle())//单播
        ui->plainTextEdit->appendPlainText(array);
    else//组播消息
        ui->plainTextEdit_2->appendPlainText(array);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //
    udpsocket=new QUdpSocket(this);

    //获取IPv4
    QList<QHostAddress> listIPv4=QNetworkInterface::allAddresses();
    for(const auto &i:listIPv4)
        if(i.isGlobal()&&i.protocol()==QAbstractSocket::IPv4Protocol)
        {
            if(!udpsocket->bind()) ui->plainTextEdit->appendPlainText("连接IP错误："+udpsocket->errorString());
            this->setWindowTitle(i.toString());
            break;
        }

    //显示绑定端口
    ui->spinBox_2->blockSignals(true);
    ui->spinBox_2->setValue(udpsocket->localPort());
    ui->spinBox_2->blockSignals(false);

    //获取组播地址输入栏全部事件
    ui->comboBox_2->installEventFilter(this);
    //this->eventFilter(ui->comboBox_2,QEvent*event);

    //建立读取信号
    connect(udpsocket,&QUdpSocket::readyRead,this,&MainWindow::do_readyRead);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if(object!=ui->comboBox_2)
        return QMainWindow::eventFilter(object,event);
    if(event->type()==QEvent::FocusOut)
    {
        //获取新的组播地址是否正确
        if(Multicast.toString()==ui->comboBox_2->currentText())
            return QMainWindow::eventFilter(object,event);
        if(QHostAddress(ui->comboBox_2->currentText()).isNull())
        {

            QMessageBox::information(this,"组播地址错误",udpsocket->errorString());
            return QMainWindow::eventFilter(object,event);
        }

        //重新加入组播
        udpsocket->leaveMulticastGroup(Multicast);
        Multicast.setAddress(ui->comboBox_2->currentText());
        if(!udpsocket->joinMulticastGroup(Multicast))
        {
            QMessageBox::information(this,"加入组播失败原因：",udpsocket->errorString());
        }
    }
    return QMainWindow::eventFilter(object,event);
}

void MainWindow::on_pushButton_clicked()
{
    //获取当前端口和对方地址
    QHostAddress ip(ui->comboBox->currentText());
    int prot=ui->spinBox->value();
    if(!prot||ip.isNull())
    {
        ui->plainTextEdit->appendPlainText("IP或者端口错误");
        return;
    }

    //获取自己ID和数据流
    QByteArray array=ui->lineEdit->text().toUtf8();

    //打包数据发送给对方
    QNetworkDatagram udpcout(array,ip,prot);
    if(udpsocket->writeDatagram(udpcout)==-1)
    {
        QMessageBox::information(this,"发送失败原因",udpsocket->errorString());
        return;
    }

    //显示发送成功后的消息
    ui->plainTextEdit->appendPlainText("我："+ui->lineEdit->text());
    ui->lineEdit->clear();
}


void MainWindow::on_pushButton_3_clicked()
{
    //获取当前端口和自己地址
    QHostAddress ip(ui->comboBox->currentText());
    int prot=ui->spinBox->value();
    if(!prot||ip.isNull())
    {
        ui->plainTextEdit->appendPlainText("IP或者端口错误");
        return;
    }

    //获取自己ID和数据流
    QByteArray array=ui->lineEdit->text().toUtf8();

    //打包数据发送给广播地址
    QNetworkDatagram udpcout(array,QHostAddress::Broadcast,prot);
    if(udpsocket->writeDatagram(udpcout)==-1)
    {
        QMessageBox::information(this,"发送失败原因",udpsocket->errorString());
    }

    //显示成功发送的消息
    ui->plainTextEdit->appendPlainText("我:"+ui->lineEdit->text());
    ui->lineEdit->clear();
}

void MainWindow::on_pushButton_2_clicked()
{
    //打包数据流,个人信息
    QNetworkDatagram data;
    data.setDestination(Multicast,ui->spinBox->value());
    data.setData(ui->lineEdit_2->text().toUtf8());

    //监测发送数据流是否成功
    if(udpsocket->writeDatagram(data)==-1)
    {
        QMessageBox::information(this,"发送组播消息失败",udpsocket->errorString());
        return;
    }

    //发送成功显示消息栏
    ui->plainTextEdit_2->appendPlainText("我"+ui->lineEdit_2->text());
    ui->lineEdit_2->clear();
}



void MainWindow::on_spinBox_2_editingFinished()
{
    if(ui->spinBox->value()==udpsocket->localPort())
        return;

    //重新开启套接字连接
    if(udpsocket->state()==QAbstractSocket::BoundState)udpsocket->close();
    if(!udpsocket->bind(QHostAddress(this->windowTitle()),ui->spinBox->value()))
    {
        ui->plainTextEdit->appendPlainText("端口被占用,随机获取接口"+udpsocket->errorString());
    }

    //自动获取端口
    if(!udpsocket->isValid())udpsocket->bind(QHostAddress(this->windowTitle()));

    //避免再调用一次
    ui->spinBox_2->blockSignals(true);
    ui->spinBox_2->setValue(udpsocket->localPort());
    ui->spinBox_2->blockSignals(false);
}


