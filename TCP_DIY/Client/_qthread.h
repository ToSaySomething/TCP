#ifndef _QTHREAD_H
#define _QTHREAD_H

#include "myclient.h"
#include <QThread>
class _Thread : public QThread
{
    Q_OBJECT
signals:
    void message(const QString& info);
    void progress(int present);

public:
    _Thread(QObject* par);
    ~_Thread();
    void setRunCount(int count);
    void run();
    void doSomething();

    void connnectsocket(std::string,int);
    QString send(std::string);
    void closesocket();
    QString receive();
private:
    int m_runCount;
    MyClient myclient;
    std::string _ipaddr;
    int _port;
    StreamSocket* _socket;
    bool flag;
};



#endif // _QTHREAD_H
