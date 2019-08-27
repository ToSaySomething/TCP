#include "work.h"

Worker::Worker(TCPServerConnectionFactory::Ptr pFactory,Poco::ThreadPool& threadpool, Params::Ptr pParams):
    _threadPool(threadpool),
    _pParams(pParams),
    _pConnectionFactory(pFactory),
    _stopped(false),
    _curthreads(0),
    _rc(1)
{
    poco_check_ptr (pFactory);

    if (!_pParams)
        _pParams = new Params;

    if (_pParams->getMaxThreads() == 0)
        _pParams->setMaxThreads(threadpool.capacity());
}

void Worker::duplicate()// add count
{
    _mutex.lock();
    ++_rc;
    _mutex.unlock();
}

void Worker::release()
{
    _mutex.lock();
    int rc = --_rc;
    _mutex.unlock();
    if (rc == 0)
        delete this;
}

void Worker::stop()
{
    _stopped = true;
    _queue.clear();
    _queue.wakeUpAll();
}

void Worker::run()
{
    AutoPtr<Worker> guard(this, true);//keep alive

    while (true)
    {
        AutoPtr<Notification> pNf(_queue.waitDequeueNotification());
        if(pNf)
        {
            WorkNotification* pWorkNf = dynamic_cast<WorkNotification*>(pNf.get());
            if (pWorkNf)
            {
                Connection1 connect(pWorkNf->socket());
                connect.run();
            }
        }
        if  (_stopped || (_curthreads > 1 && _queue.empty()) )
        {//STOP or no_curthreads or no empty queue
            --_curthreads;
            break;
        }
    }
}

void Worker::enqueue(const StreamSocket& ss)
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    try
    {
//        std::cout << " enqueue: " << std::endl;
        _queue.enqueueNotification(new WorkNotification(ss));
        _threadPool.start(*this);
        _curthreads++;
    }
    catch(Poco::Exception&e)
    {
    }
}
