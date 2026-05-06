#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHostInfo>
#include <QNetworkInterface>
#include <QMessageBox>
#include <QByteArray>

void MainWindow::do_disconnected()
{
    //删除数据
    QTcpSocket* tcp_id=static_cast<QTcpSocket*>(sender());
    QUuid uuid=temporaryID.key(tcp_id);
    temporaryID.remove(temporaryID.key(tcp_id));
    tcp_id->deleteLater();

    //删除显示页面数据
    ui->comboBox->setEditable(false);
    ui->comboBox->setCurrentText(uuid.toString(QUuid::WithoutBraces));
    ui->comboBox->removeItem(ui->comboBox->currentIndex());
    ui->comboBox->setEditable(true);
}

void MainWindow::do_readyRead()//处理客户端信息
{
    QString str;
    //获取那个客户端发消息
    QTcpSocket* id=static_cast<QTcpSocket*>(sender());
    if(id==nullptr)
    {
        this->Error(id,str="客户端丢失,请联系开发者");
        return;
    }

    //获取ID和数据流
    QUuid uuid;
    QByteArray array=id->readAll(); //获取全部信息

    uuid=this->Handle(array);
    if(!temporaryID.contains(uuid))
    {
        this->Error(id,"没有这ID，请重新发送");
        return;
    }

    //获取发送人的ID
    QString strcout; //获取数据流
    strcout=temporaryID.key(id).toString(QUuid::WithoutBraces)+':';
    array.insert(0,strcout.toUtf8());
    this->Client_ID(uuid,array);
}

void MainWindow::do_newConnection()
{
    ui->plainTextEdit->appendPlainText("检测到有客户端连接服务器");
}

void MainWindow::do_acceptError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);
    ui->plainTextEdit->appendPlainText("连接出现问题原因："+tcpserver->errorString());
}

void MainWindow::do_pendingConnectionAvailable()
{
    //测试功能位置
    ui->comboBox->setEditable(false);

    //连接成功
    if(!tcpserver->hasPendingConnections())
    {
        ui->plainTextEdit->appendPlainText("没有获取客户端问题原因："+tcpserver->errorString());
        return;
    }

    //伪随机ID,唯一存入连接客户端ID
    QUuid uuid=QUuid::createUuid();
    QTcpSocket* tcp_ID=tcpserver->nextPendingConnection();
    temporaryID.insert(uuid,tcp_ID);

    //发送iD给客户端
    QByteArray array=uuid.toString().toUtf8();
    if(-1==tcp_ID->write(array))
        ui->plainTextEdit->appendPlainText("发送失败"+uuid.toString()+tcp_ID->errorString());

    //显示连接开发端的IP4地址和端口
    ui->plainTextEdit->appendPlainText("新客户端地址，和端口："+tcp_ID->peerAddress().toString()+'\n'+QString("%1").arg(tcp_ID->peerPort()));

    //加入uiID去
    ui->comboBox->addItem(uuid.toString(QUuid::WithoutBraces));

    //建立信号连接
    connect(tcp_ID,&QTcpSocket::readyRead,this,&MainWindow::do_readyRead);//获取客户发送信息
    connect(tcp_ID,&QTcpSocket::disconnected,this,&MainWindow::do_disconnected);//客户要退出程序

    //测试
    ui->comboBox->setEditable(true);
}

QUuid MainWindow::Handle(QByteArray &flow)//分耕iD和数据流
{
    QUuid uuid;
    qint64 id_uu=uuid.toString(QUuid::WithoutBraces).toUtf8().size();//获取iD

    //如果ID小于或者ID不正确时
    if(flow.size()<id_uu)
    {
        return QUuid();
    }

    uuid=QUuid::fromString(QString::fromStdString(flow.first(id_uu).toStdString()));

    flow.remove(0,id_uu);
    return uuid;
}

void MainWindow::Client_ID(QUuid id, QByteArray &flow)
{
    if(!temporaryID.contains(id))
    {
        return;
    }

    //向客户端发送信息
    QTcpSocket* tcp_ID=temporaryID[id];
    tcp_ID->write(flow);
}

void MainWindow::Error(QTcpSocket *id, QString error)
{
    if(error.isEmpty())
        return;

    QByteArray array=error.toUtf8();
    if(id->write(array)==-1)
    {
        ui->plainTextEdit->appendPlainText(temporaryID.key(id).toString()+id->errorString());
        return;
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //获取内存地址
    tcpserver=new QTcpServer(this);
    ID=new QTcpSocket(this);
    label=new QLabel(this);
    port=new QLabel(this);

    //初始化ui
    ui->statusbar->addWidget(label,250);
    ui->statusbar->addWidget(port,250);

    //window页面显示真实IPV4地址
    QList<QHostAddress> IPv4=QNetworkInterface::allAddresses();
    QList<QHostAddress> listIPv4;
    for(const auto &i:IPv4)
        if(i.isGlobal()&&(i.protocol()==QAbstractSocket::IPv4Protocol))
            listIPv4.append(i);
    for(const auto &i:IPv4)
        if(i.isLoopback()&&(i.protocol()==QAbstractSocket::IPv4Protocol))
            listIPv4.append(i);

    //显示状态
    this->setWindowTitle("没有绑定IP");
    label->setText("服务器为开启:");

    //每当有客户端连接服务端时
    connect(tcpserver,&QTcpServer::acceptError,this,&MainWindow::do_acceptError);
    connect(tcpserver,&QTcpServer::newConnection,this,&MainWindow::do_newConnection);
    connect(tcpserver,&QTcpServer::pendingConnectionAvailable,this,&MainWindow::do_pendingConnectionAvailable);

}

MainWindow::~MainWindow()
{
    this->on_action_2_triggered();
    delete ui;
}

void MainWindow::on_action_1_triggered()
{
    //如果服务器已开启
    if(tcpserver->isListening())
        return;

    //获取全局IPv4,和回环地址
    QList<QHostAddress> IPv4=QNetworkInterface::allAddresses();
    QList<QHostAddress> listIPv4;
    for(const auto &i:IPv4)
        if(i.isGlobal()&&(i.protocol()==QAbstractSocket::IPv6Protocol))
            listIPv4.append(i);
    for(const auto &i:IPv4)
        if(i.isLoopback()&&(i.protocol()==QAbstractSocket::IPv4Protocol))
            listIPv4.append(i);

    // 启动服务器监听
    if(!tcpserver->listen())
    {
        QMessageBox::information(this,"启动失败原因:",tcpserver->errorString());
        return;
    }

    /*设置本机客户端地址和端口
    if(!ID->bind(listIPv4.at(1)))
    {
        qDebug()<<"端口已被占用";
        return ;
    }*/

    //测试客户端口
    connect(ID,&QTcpSocket::readyRead,this,[this]{ui->plainTextEdit->appendPlainText(ID->readAll());});
    connect(ID,&QTcpSocket::errorOccurred,this,[this]{ui->plainTextEdit->appendPlainText("测试客户端问题："+ID->errorString());});


    //测试客户端连接服务器
    ID->connectToHost(listIPv4.at(0), tcpserver->serverPort());

    //显示IP,监听状态，端口
    this->setWindowTitle(tcpserver->serverAddress().toString());
    label->setText("监听状态：正在监听");
    port->setText(QString("%1").arg(tcpserver->serverPort()));

    //修改ui显示页面
    ui->action_1->setEnabled(false);
    ui->action_2->setEnabled(true);
    ui->pushButton->setEnabled(true);


}


void MainWindow::on_action_2_triggered()
{
    QList<QTcpSocket*> id=temporaryID.values();
    for(auto i:id)
        i->disconnectFromHost();

    // 关闭服务器
    tcpserver->close();

    //显示消息
    this->setWindowTitle("没有绑定IP");
    label->setText("监听状态:停止监听");
    port->clear();

    //清空下拉shi
    ui->comboBox->clear();
    ui->comboBox->addItem("给全部上线ID发信息");

    //
    ui->action_1->setEnabled(true);
    ui->action_2->setEnabled(false);
    ui->pushButton->setEnabled(false);
}


void MainWindow::on_pushButton_clicked()
{
    //输出框没有消息
    if(ui->lineEdit->text().isEmpty())
        return;
    //
    QUuid uuid(ui->comboBox->currentText());

    //给全部人发消息
    if(ui->comboBox->currentText()=="给全部上线ID发信息")
    {

        //获取ID客户端
        QList<QTcpSocket*> list_id=temporaryID.values();
        for(const auto &i:list_id)
        {
            if(i==ID)
                continue;

            //
            QString strcout;
            strcout="属于官方ID:";
            QByteArray attay=strcout.toUtf8()+ui->lineEdit->text().toUtf8();
            if(i->write(attay)==-1)
                qDebug()<<"出错原因"+i->errorString();
        }
    }//获取到ID并不存在时
    else if(temporaryID.contains(uuid))
    {

        QTcpSocket* id=temporaryID.value(QUuid(ui->comboBox->currentText()));
        if(id!=nullptr)
        {
            //
            QString strcout;
            strcout="属于官方ID(私发):";
            //uuid_ID.toString(QUuid::WithoutBraces)
            QByteArray attay=strcout.toUtf8()+ui->lineEdit->text().toUtf8();
            if(id->write(attay)==-1)
                qDebug()<<"出错原因"+id->errorString();
        }
    }
}
