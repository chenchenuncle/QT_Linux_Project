#include "mainwindow.h"

#include <QApplication>
#include <QInputDialog>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget get;
    QString iP=QInputDialog::getText(&get,"输入栏","请输入IP");
    qint16 port=QInputDialog::getInt(&get,"输入服务器端口","请输入");
    MainWindow w(iP,port);
    w.show();
    return a.exec();
}
