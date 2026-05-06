#include "thread_read.h"
#include <QSemaphore>
#include <QMutex>

int buff1[10],buff2[10],buff_sum=0,buff_id=1;//2个缓冲区，1个数据，1个缓冲区序号
int buff_close=1;//清空的缓冲序号互换
int buff_start=1;//空间缓冲区序号互换
QSemaphore semaphore(2);
QSemaphore phroeclose(0);


void thread_read::run()
{
    while(ol_start)
    {
        semaphore.acquire(1);
        int* buff=buff_start==1?buff1:buff2;
        QVector<int> sum(10);
        for(int i=0;i<10;i++)
        {
            *(buff+i)=buff_sum;//写数据
            sum[i]=buff_sum++;
        }
        emit num(buff_id++,sum);
        buff_start=buff_start==1?2:1;
        phroeclose.release(1);
        msleep(1000);
    }
    //this->exec();
}

thread_read::thread_read() {}


void thread_close::run()
{
    while(ol_start)
    {
        phroeclose.acquire(1);
        int* buff=buff_close==1?buff1:buff2;
        for(int i=0;i<10;i++)
        {
            *(buff+i)=0;//清空缓冲区数据
        }
        buff_close=buff_close==1?2:1;
        semaphore.release(1);
        msleep(1000);
    }
    //this->exec();
}
