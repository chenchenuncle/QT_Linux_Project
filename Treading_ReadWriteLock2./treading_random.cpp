#include "treading_random.h"
#include <QRandomGenerator64>
#include <QTimer>
QReadWriteLock lock;
QWaitCondition tion;
int sum=0;int dice=0;

void Treading_random::run()
{
    while(1)
    {
        //锁定住
        lock.lockForWrite();
        //随机数，总数++
        dice=QRandomGenerator64::global()->bounded(1,7);
        sum+=1;
        lock.unlock();
        tion.wakeAll();
        sleep(1);
    }
}


void Treading_read_int::run()
{
    while(1)
    {
        //锁读
        lock.lockForRead();
        tion.wait(&lock);
        emit qo_read(sum,dice);
        lock.unlock();
    }
}

void Treading_read_str::run()
{
    while(1)
    {
        lock.lockForRead();
        tion.wait(&lock);
        emit qo_read(QString::asprintf(":/dice/dice_%d.png",dice));
        lock.unlock();
    }
}
