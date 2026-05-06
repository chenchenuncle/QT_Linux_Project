#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::do_num(int id_suff,QVector<int> sum)
{
    QString str=QString::asprintf("第%d个缓冲区:",id_suff);
    for(int i=0;i<10;i++)
       str+=QString::asprintf("  %d",sum[i]);//获取缓冲区的数据
    ui->plainTextEdit->appendPlainText(str);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->plainTextEdit);
    buff_read=new thread_read;
    buff_clos=new thread_close;

    connect(buff_read,&thread_read::num,this,&::MainWindow::do_num);
}

MainWindow::~MainWindow()
{
    on_action_4_triggered();

    delete ui;
}

void MainWindow::on_action_1_triggered()
{
    //开启循环事件
    buff_read->is_start(true);
    buff_clos->is_start(true);

    //占用线程
    buff_read->start();
    buff_clos->start();

    //gaibianzujian
    ui->action_1->setEnabled(false);
    ui->action_4->setEnabled(true);
}


void MainWindow::on_action_4_triggered()
{
    //关闭循环事件
    buff_read->is_start(false);
    buff_clos->is_start(false);

    //
    ui->action_1->setEnabled(true);
    ui->action_4->setEnabled(false);
}

