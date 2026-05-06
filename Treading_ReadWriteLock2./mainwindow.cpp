#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::do_int(int sum, int dice)
{
    QString str=QString::asprintf("第%d次抛骰子,点数为%d",sum,dice);
    ui->textEdit->append(str);
}

void MainWindow::do_str(QString str)
{
    //图片大小更随组件大小
    QPixmap pixmap(str);
    ui->label->setPixmap(pixmap.scaled(ui->label->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    label=new QLabel(this);
    trand_dice=new Treading_random;
    read_int=new Treading_read_int;
    read_str=new Treading_read_str;

    ui->statusbar->addWidget(label,250);
    label->setText("子线程未开启");

    //建立信号获取随机骰子
    connect(read_int,&Treading_read_int::qo_read,this,&MainWindow::do_int);
    connect(read_str,&Treading_read_str::qo_read,this,&MainWindow::do_str);
}

MainWindow::~MainWindow()
{
    if(trand_dice->isRunning())
        trand_dice->wait();
    if(read_int->isRunning())
        trand_dice->wait();
    if(read_str->isRunning())
        trand_dice->wait();
    delete ui;
}

void MainWindow::on_action_1_triggered()
{
    //启动线程
    read_int->start();
    read_str->start();
    Treading_random::msleep(200);
    trand_dice->start();
    //激活组件
    ui->action_1->setEnabled(false);
    ui->action_4->setEnabled(true);

    //显示自线程状态
    label->setText("占用子线程");
}


void MainWindow::on_action_4_triggered()
{
    //结束run事件循环
    trand_dice->terminate();
    trand_dice->wait();

    /*
    read_int->terminate();
    read_int->wait();
    read_str->terminate();
    read_str->wait();
    */


    //激活组件
    ui->action_1->setEnabled(true);
    ui->action_4->setEnabled(false);

    //显示自线程状态
    label->setText("释放子线程");
}

