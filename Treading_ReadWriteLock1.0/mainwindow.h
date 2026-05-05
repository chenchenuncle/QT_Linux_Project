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

public slots:
    void do_dice(QPixmap pixmap,int sum,int dice);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_action_1_triggered();

    void on_action_2_triggered();

    void on_action_3_triggered();

    void on_action_4_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
