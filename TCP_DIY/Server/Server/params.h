#ifndef PARAMS_H
#define PARAMS_H

#include "Poco/Net/Net.h"
#include "Poco/RefCountedObject.h"
#include "Poco/Timespan.h"
#include "Poco/Thread.h"
#include "Poco/AutoPtr.h"

class  Params: public Poco::RefCountedObject
{
public:
    typedef Poco::AutoPtr<Params> Ptr;
    Params();
    ~Params();

public:
    void setThreadIdleTime(const Poco::Timespan& idleTime);
    const Poco::Timespan& getThreadIdleTime();
    void setMaxQueued(int count);
    int getMaxQueued();
    void setMaxThreads(int count);
    int getMaxThreads();
    void setThreadPriority(Poco::Thread::Priority prio);
    Poco::Thread::Priority getThreadPriority();

private:
    Poco::Timespan _threadIdleTime;
    Poco::Thread::Priority _threadPriority;
    int _maxThreads;
    int _maxQueued;
};

#endif // PARAMS_H
