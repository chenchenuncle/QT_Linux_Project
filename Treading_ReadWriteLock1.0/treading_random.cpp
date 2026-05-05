#include "treading_random.h"
#include "qdebug.h"
#include "qpixmap.h"
#include <QRandomGenerator64>
#include <QTimer>

void Treading_random::do_start(bool is)
{
    Start=is;
}

void Treading_random::do_Pause(bool is)
{
    Pause=is;
}

void Treading_random::run()
{
    //是否开启线程
    while(Start)
    {
        //是否开启开始按钮
        if(Pause)
        {
            //获取随机数字
            int dice=QRandomGenerator64::global()->bounded(1,7);
            QString str=QString::asprintf(":/dice/dice_%d.png",dice);

            //总数加一
            sum+=1;

            //返回图片和总数
            emit qo_dice(QPixmap(str),sum,dice);

            //运行一秒再退出
            QTimer mer;
            mer.start(1000);
            while(mer.remainingTime()>0);
        }
    }
    exec();
}

Treading_random::Treading_random() {}
