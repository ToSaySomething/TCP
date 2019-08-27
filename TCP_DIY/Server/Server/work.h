#ifndef WORK_H
#define WORK_H


#include "worknotification.h"
#include "_appconnection.h"
#include "params.h"
#include "Poco/Runnable.h"
#include "Poco/AutoPtr.h"
#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/ThreadPool.h"
#include <iostream>

using Poco::Runnable;
using Poco::AutoPtr;
using Poco::Net::TCPServerConnectionFactory;

class Worker: public Runnable
{
    const long long LEN = 1024;
    const long long TIME = 200000;
public:
    Worker(TCPServerConnectionFactory::Ptr pFactory,Poco::ThreadPool& threadpool, Params::Ptr pParams);

    void run();

public:
    void duplicate();
    void release();//deletes
    void stop();
    void enqueue(const StreamSocket& ss);

private:
    NotificationQueue _queue;
    StreamSocket _socket;
    mutable Poco::FastMutex _mutex;
    Poco::ThreadPool& _threadPool;
    Params::Ptr _pParams;
    Poco::Net::TCPServerConnectionFactory::Ptr _pConnectionFactory;

    bool _stopped;
    int _curthreads;
    int _rc;

};


#endif // WORK_H
