#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "thread_read.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    thread_read* buff_read;
    thread_close* buff_clos;

public slots:
    void do_num(int id_suff,QVector<int>);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_action_4_triggered();

    void on_action_1_triggered();

private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
