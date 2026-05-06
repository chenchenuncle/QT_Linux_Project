#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qlabel.h"
#include "treading_random.h"
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
    QLabel* label;
    Treading_random* trand_dice;
    Treading_read_int* read_int;
    Treading_read_str* read_str;

public slots:
    void do_int(int sum,int dice);
    void do_str(QString str);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_action_1_triggered();

    void on_action_4_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
