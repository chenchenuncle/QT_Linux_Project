#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>
#include <QtConcurrentRun>
#include <QMessageBox>
#include <QTimer>

void MainWindow::do_errorOccurred(QSerialPort::SerialPortError error)
{
    switch ((int)error) {
    case 0:ui->textEdit->append("没有错误");break;
    case 1:ui->textEdit->append("尝试打开不存在的设备时发生错误");break;
    case 2:ui->textEdit->append("尝试打开已被其他进程打开的设备时发生错误，或用户没有足够的权限和凭证打开设备时发生错误。");break;
    case 3:ui->textEdit->append("尝试打开此对象中已打开的设备时发生错误");break;
    case 4:ui->textEdit->append("写入数据时发生 I/O 错误。");break;
    case 5:ui->textEdit->append("读取数据时发生 I/O 错误");break;
    case 6:ui->textEdit->append("资源不可用时发生 I/O 错误，例如设备意外从系统中删除。");break;
    case 7:ui->textEdit->append("运行中的操作系统不支持或禁止所请求的设备操作");break;
    case 8:ui->textEdit->append("发生不明错误");break;
    case 9:ui->textEdit->append("发生超时错误。");break;
    case 10:ui->textEdit->append("当执行的操作只有在设备已打开的情况下才能成功执行时，会出现此错误。");break;
    }
}

void MainWindow::do_readyRead()
{
    if(port->isOpen())
    {
        QByteArray array=port->readAll();
        QString str(array);
        ui->textEdit->append(str);
    }
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    //如果标签是点击按钮
    for(const auto &i:labels)
        if(i==watched&&event->type()==QEvent::MouseButtonRelease)
        {
            QByteArray array=QString(static_cast<QLineEdit*>(i->buddy())->text()+"\r\n").toUtf8();
            if(port->write(array)==-1)
                ui->textEdit->append("发送失败");
        }
    return QMainWindow::eventFilter(watched,event);
}

QSerialPort::StopBits MainWindow::stopbits(float i)
{
    if(i==1.5)
        return QSerialPort::StopBits::OneAndHalfStop;
    else if(i==2.0)
        return QSerialPort::StopBits::TwoStop;
    return QSerialPort::StopBits::OneStop;
}

void MainWindow::ion()
{
    labels.append(ui->label_11);
    labels.append(ui->label_12);
    labels.append(ui->label_13);
    labels.append(ui->label_14);
    labels.append(ui->label_15);
    labels.append(ui->label_16);
    labels.append(ui->label_17);
    labels.append(ui->label_18);
    labels.append(ui->label_19);
    labels.append(ui->label_20);
    labels.append(ui->label_21);
    labels.append(ui->label_22);

    ui->label_11->installEventFilter(this);
    ui->label_12->installEventFilter(this);
    ui->label_13->installEventFilter(this);
    ui->label_14->installEventFilter(this);
    ui->label_15->installEventFilter(this);
    ui->label_16->installEventFilter(this);
    ui->label_17->installEventFilter(this);
    ui->label_18->installEventFilter(this);
    ui->label_19->installEventFilter(this);
    ui->label_20->installEventFilter(this);
    ui->label_21->installEventFilter(this);
    ui->label_22->installEventFilter(this);

    for(const auto &i:labels)
        i->setMouseTracking(true);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //
    port=new QSerialPort(this);

    //初始化ui（子线程)
    (void)QtConcurrent::run([this](){
        //获取端口和波特率
        QList<QSerialPortInfo> infos=QSerialPortInfo::availablePorts();
        for(const auto &i:infos) ui->comboBox_2->addItem(i.portName());
        QList<qint32> sums=QSerialPortInfo::standardBaudRates();
        for(const auto &i:sums) ui->comboBox_3->addItem(QString("%1").arg(i));
        ui->comboBox_2->setCurrentIndex(0);
        ui->comboBox_3->setCurrentIndex(0);
    });

    /*获取端口和波特率
    QList<QSerialPortInfo> infos=QSerialPortInfo::availablePorts();
    for(const auto &i:infos) ui->comboBox_2->addItem(i.portName());
    QList<qint32> sums=QSerialPortInfo::standardBaudRates();
    for(const auto &i:sums) ui->comboBox_3->addItem(QString("%1").arg(i));
    ui->comboBox_2->setCurrentIndex(0);
    ui->comboBox_3->setCurrentIndex(0);*/

    //建立信号和槽
    connect(port,&QSerialPort::errorOccurred,this,&MainWindow::do_errorOccurred);
    connect(port,&QSerialPort::readyRead,this,&MainWindow::do_readyRead);

    this->ion();
}

MainWindow::~MainWindow()
{
    if(port->isOpen())
        port->close();
    delete ui;
}

void MainWindow::on_action_1_triggered()
{
    if(port->isOpen())
        port->close();

    /*建立设备通道(子线程)
    (void)QtConcurrent::run([this](){
        port->setPortName(ui->comboBox_2->currentText());
        port->setBaudRate(ui->comboBox_3->currentText().toLongLong());
        port->setDataBits((QSerialPort::DataBits)ui->comboBox_4->currentText().toInt());
        port->setStopBits(this->stopbits(ui->comboBox_5->currentText().toFloat()));
        if(ui->comboBox_6->currentIndex()==0)
            port->setParity(QSerialPort::Parity(0));
        else
            port->setParity(QSerialPort::Parity(ui->comboBox_6->currentIndex()+1));

        //打开串口
        if(!port->open(QIODeviceBase::ReadWrite))
        {
            QMessageBox::information(this,"打不开设备",port->errorString());
            return;
        }
    });*/

    port->setPortName(ui->comboBox_2->currentText());
    port->setBaudRate(ui->comboBox_3->currentText().toLongLong());
    port->setDataBits((QSerialPort::DataBits)ui->comboBox_4->currentText().toInt());
    port->setStopBits(this->stopbits(ui->comboBox_5->currentText().toFloat()));
    if(ui->comboBox_6->currentIndex()==0)
        port->setParity(QSerialPort::Parity(0));
    else
        port->setParity(QSerialPort::Parity(ui->comboBox_6->currentIndex()+1));

    //打开串口
    if(!port->open(QIODeviceBase::ReadWrite))
    {
        QMessageBox::information(this,"打不开设备",port->errorString());
        return;
    }

    //修改ui页面
    ui->comboBox->setCurrentText(ui->comboBox_3->currentText());
    ui->action_1->setEnabled(false);
    ui->action_2->setEnabled(true);
    ui->comboBox_2->setEnabled(false);
    ui->comboBox_3->setEnabled(false);
    ui->comboBox_4->setEnabled(false);
    ui->comboBox_5->setEnabled(false);
    ui->comboBox_6->setEnabled(false);
}


void MainWindow::on_action_2_triggered()
{
    //如果已经关闭开闭串口
    if(!port->isOpen())
        return;

    //
    port->close();

    //修改ui
    ui->action_1->setEnabled(true);
    ui->action_2->setEnabled(false);
    ui->comboBox_2->setEnabled(true);
    ui->comboBox_3->setEnabled(true);
    ui->comboBox_4->setEnabled(true);
    ui->comboBox_5->setEnabled(true);
    ui->comboBox_6->setEnabled(true);
}


void MainWindow::on_pushButton_clicked()
{
    //发送消息
    QByteArray array("AT\r\n");
    ui->textEdit->append("AT");
    if(port->write(array)==-1)
        ui->textEdit->append("发送失败");
}


void MainWindow::on_pushButton_2_clicked()
{
    //发送消息
    QByteArray array("AT+RST\r\n");
    if(port->write(array)==-1)
        ui->textEdit->append("发送失败");
}


void MainWindow::on_pushButton_16_clicked()
{
    QByteArray array=ui->textEdit_2->toPlainText().toUtf8()+"\r\n";
    if(port->write(array)==-1)
        QMessageBox::information(this,"发送失败",port->errorString());
}


void MainWindow::on_pushButton_3_clicked()
{
    //发送消息
    QByteArray array("AT+RESTORE\r\n");
    if(port->write(array)==-1)
        ui->textEdit->append("发送失败");
}


void MainWindow::on_pushButton_4_clicked()
{
    //发送消息
    QByteArray array(QString("AT+UART_CUR="+ui->comboBox->currentText()+",8,1,0,0\r\n").toUtf8());
    if(port->write(array)==-1)
        ui->textEdit->append("发送失败");
}


void MainWindow::on_pushButton_5_clicked()
{
    //发送消息
    QByteArray array("AT+UART_CUR?\r\n");
    if(port->write(array)==-1)
        ui->textEdit->append("发送失败");
}


void MainWindow::on_pushButton_6_clicked()
{
    //发送消息
    if(ui->lineEdit_2->text().isEmpty()||ui->lineEdit->text().isEmpty())
        return;
    QByteArray array1("AT+CWMODE=1,1\r\n");
    if(port->write(array1)==-1)
        ui->textEdit->append("发送失败");

    QTimer::singleShot(500,this,[this](){
        QByteArray array2(QString("AT+CWJAP=\""+ui->lineEdit->text()+"\",\""+ui->lineEdit_2->text()+"\"\r\n").toUtf8());
        if(port->write(array2)==-1)
            ui->textEdit->append("发送失败");
    });
}


void MainWindow::on_pushButton_7_clicked()
{
    //发送消息
    QByteArray array1("AT+CWMODE=3,1\r\n");
    if(port->write(array1)==-1)
        ui->textEdit->append("发送失败");

    QTimer::singleShot(1000,this,[this](){
        QByteArray array2("AT+CWSAP=\"全国可飞~\",\"7777777777\",1,3\r\n");
        if(port->write(array2)==-1)
            ui->textEdit->append("发送失败");
    });
}

void MainWindow::on_pushButton_8_clicked()
{
    //发送消息
    QByteArray array(QString("AT+CWSAP?\r\n").toUtf8());
    if(port->write(array)==-1)
       ui->textEdit->append("发送失败");
}

void MainWindow::on_pushButton_9_clicked()
{
    //发送消息
    if(ui->lineEdit_3->text().isEmpty()&&ui->spinBox->value()==0)
        return;
    /*QByteArray array1("AT+CWMODE=1,1\r\n");
    if(port->write(array1)==-1)
        ui->textEdit->append("发送失败");*/

    QTimer::singleShot(200,this,[this](){
        QByteArray array2(QString("AT+CIPSTART=\"TCP\",\""+ui->lineEdit_3->text()+"\","+QString("%1").arg(ui->spinBox->value())+",30\r\n").toUtf8());
        if(port->write(array2)==-1)
            ui->textEdit->append("发送失败");
    });
}


void MainWindow::on_pushButton_10_clicked()
{
    QByteArray cmd="AT+CWAUTOCONN=1\r\n";
    if(port->write(cmd)==-1)
        ui->textEdit->append("发送失败");
}


void MainWindow::on_pushButton_11_clicked()
{
    //发送消息
    QByteArray array(QString("AT+CIPSTATUS\r\n").toUtf8());
    if(port->write(array)==-1)
        ui->textEdit->append("发送失败");
}


void MainWindow::on_pushButton_12_clicked()
{
    //发送消息
    QByteArray array(QString("AT+CIFSR\r\n").toUtf8());
    if(port->write(array)==-1)
        ui->textEdit->append("发送失败");
}


void MainWindow::on_pushButton_13_clicked()
{
    //发送消息
    QByteArray array(QString("+++").toUtf8());
    if(port->write(array)==-1)
        ui->textEdit->append("发送失败");
}


void MainWindow::on_pushButton_14_clicked()
{
    //设置透传前制要条件
    QByteArray array(QString("AT+CIPMUX=0\r\n").toUtf8());
    if(port->write(array)==-1)
        ui->textEdit->append("发送失败");

    QTimer::singleShot(600,this,[this](){
        QByteArray array2(QString("AT+CIPMODE=1\r\n").toUtf8());
        if(port->write(array2)==-1)
            ui->textEdit->append("发送失败");
    });

    //发送消息
    QTimer::singleShot(1500,this,[this](){
    QByteArray array(QString("AT+CIPSEND\r\n").toUtf8());
    if(port->write(array)==-1)
        ui->textEdit->append("发送失败");
    });
}

void MainWindow::on_pushButton_15_clicked()
{
    //发送消息
    QByteArray array(QString("AT+SAVETRANSLINK=1,\"10.50.106.199\",2721,\"TCP\",300\r\n").toUtf8());
    if(port->write(array)==-1)
        ui->textEdit->append("发送失败");

    //发送消息
    QTimer::singleShot(500,this,[this](){
        QByteArray array(QString("AT+CIPSEND\r\n").toUtf8());
        if(port->write(array)==-1)
            ui->textEdit->append("发送失败");
    });
}

