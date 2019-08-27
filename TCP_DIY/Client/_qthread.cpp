#include "_qthread.h"
#include <QDebug>
#include <iostream>

_Thread::_Thread(QObject* par) :
    QThread(par),
    _socket(nullptr),
    m_runCount(100)
{
    flag = true;
}

_Thread::~_Thread()
{
    qDebug() << "~Thread";
}

void _Thread::setRunCount(int count)
{
    m_runCount = count;
}

void _Thread::run()
{
    int count = 0;
    while(1)
    {
        sleep(1);
        ++count;
        doSomething();
        if(m_runCount == count)
        {
            break;
        }
    }
}

void _Thread::doSomething()
{
    msleep(500);
    myclient.send_heardbeat();
}

void _Thread::connnectsocket(std::string ipaddr, int Port)
{
    std::string ip = ipaddr;
    int port = Port;
    myclient.myconnect(ip, port);
}

QString _Thread::send(std::string str)
{
    QString re;
    if(flag)
    {
        flag = false;
        re = myclient.send(str);
    }
    else
    {
        flag = true;
    }
    return re;

}

QString _Thread::receive()
{
    return myclient.receive();
}

void _Thread::closesocket()
{
    myclient.close();
}



