#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::do_dice(QPixmap pixmap, int sum, int dice)
{
    //图片大小更随组件大小
    ui->label->setPixmap(pixmap.scaled(ui->label->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    QString str=QString::asprintf("第%d次抛骰子,点数为%d",sum,dice);
    ui->textEdit->append(str);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    label=new QLabel(this);
    trand_dice=new Treading_random;

    ui->statusbar->addWidget(label,250);
    label->setText("子线程未开启");

    //建立信号获取随机骰子
    connect(trand_dice,&Treading_random::qo_dice,this,&MainWindow::do_dice);
}

MainWindow::~MainWindow()
{
    if(trand_dice->isMainThread())
        trand_dice->wait();

    delete ui;
}

void MainWindow::on_action_1_triggered()
{
    //进入run事件循环
    trand_dice->do_start(true);

    //占用线程
    trand_dice->start();

    //激活组件
    ui->action_1->setEnabled(false);
    ui->action_2->setEnabled(true);
    ui->action_3->setEnabled(false);
    ui->action_4->setEnabled(true);

    //显示自线程状态
    label->setText("占用子线程");
}


void MainWindow::on_action_2_triggered()
{
    //占用子线程,开始骰子
    trand_dice->do_Pause(true);

    //激活组件
    ui->action_2->setEnabled(false);
    ui->action_3->setEnabled(true);

    //显示自线程状态
    label->setText("占用子线程并使用它");
}


void MainWindow::on_action_3_triggered()
{
    //占用子线程,停止骰子
    trand_dice->do_Pause(false);

    //激活组件
    ui->action_2->setEnabled(true);
    ui->action_3->setEnabled(false);

    //显示自线程状态
    label->setText("占用子线程");
}


void MainWindow::on_action_4_triggered()
{
    //结束run事件循环
    trand_dice->do_start(false);

    //释放子线程
    trand_dice->terminate();

    //激活组件
    ui->action_1->setEnabled(true);
    ui->action_2->setEnabled(false);
    ui->action_3->setEnabled(false);
    ui->action_4->setEnabled(false);

    //显示自线程状态
    label->setText("释放子线程");
}

