#ifndef THREAD_READ_H
#define THREAD_READ_H

#include <QThread>

class thread_read : public QThread
{
    Q_OBJECT


private:
    bool ol_start;

protected:
    virtual void run();

signals:
    void num(int,QVector<int>);

public:
    void is_start(bool is){ol_start=is;};

public:
    thread_read();
};

class thread_close : public QThread
{
    Q_OBJECT
private:
    bool ol_start;

protected:
    virtual void run();

public:
    void is_start(bool is){ol_start=is;}

public:
    thread_close(){};
};
#endif // THREAD_READ_H
