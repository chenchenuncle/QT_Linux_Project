#ifndef TREADING_RANDOM_H
#define TREADING_RANDOM_H

#include <QThread>
#include <QPixmap>
#include <QWaitCondition>
#include <QReadWriteLock>



class Treading_random : public QThread
{
    Q_OBJECT
protected:
    virtual void run();//写入数据
};

class Treading_read_int : public QThread
{
    Q_OBJECT

signals:
    void qo_read(int,int);
protected:
    virtual void run();//传入骰子
};
class Treading_read_str : public QThread
{
    Q_OBJECT
signals:
    void qo_read(QString);
protected:
    virtual void run();//传入图片
};

#endif // TREADING_RANDOM_H
