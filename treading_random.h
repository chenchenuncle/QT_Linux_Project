#ifndef TREADING_RANDOM_H
#define TREADING_RANDOM_H

#include <QThread>
#include <QPixmap>

class Treading_random : public QThread
{
    Q_OBJECT

private:
    int sum=0;//骰子总数
    bool Start=false;//run事件结束按钮
    bool Pause=false;//进行骰子事件按钮

signals:
    void qo_dice(QPixmap pixmap,int sum,int x);


public slots:
    void do_start(bool);
    void do_Pause(bool);

protected:
    virtual void run();//执行函数

public:
    Treading_random();
};

#endif // TREADING_RANDOM_H
