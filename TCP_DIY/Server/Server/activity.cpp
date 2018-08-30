#include "activity.h"

using Poco::Net::ServerSocket;


MyActivity::MyActivity(TCPServerConnectionFactory::Ptr pFactory, const ServerSocket& socket, Params::Ptr pParams):
    _activity(this, &MyActivity::runActivity),
    _socket(socket)
{
    Poco::ThreadPool& pool = Poco::ThreadPool::defaultPool();
    if (pParams)
    {//add
        int toAdd =  pParams->getMaxThreads() - pool.capacity();
        if (toAdd > 0) pool.addCapacity(toAdd);
    }
    _worker = new Worker(pFactory, pool, pParams);
}

MyActivity::~MyActivity()
{
    try
    {
        stop();
        _worker->release();
    }
    catch (...)
    {
        poco_unexpected();
    }
}

void MyActivity::start()
{
    _activity.start();
}

void MyActivity::stop()
{
    _activity.stop();
    _activity.wait();
}


void MyActivity::runActivity()
{
    while(!_activity.isStopped())
    {
        //std::cout << " --- activity: " << std::endl;
        Poco::Timespan timeout(TIME);

        try
        {
            bool flag = _socket.poll(timeout, Poco::Net::Socket::SELECT_READ);
            if (flag)
            {
                try
                {
                    StreamSocket ss = _socket.acceptConnection();
                    ss.setNoDelay(true);
                    //queue.enqueueNotification(new WorkNotification(ss));
                    _worker->enqueue(ss);

                }
                catch(Poco::Exception& e)
                {
                    std::cout<< " Exception"<< e.what() << std::endl;
                }
            }
           //std::cout << " --- end activity : " << std::endl << std::endl;
        }
        catch(Poco::Exception& e)
        {
            std::cout<< " Exception"<< e.what() << std::endl;
            Thread::sleep(50);
        }
    }
}


/*
#include "activity.h"
#include "thread.h"

using Poco::Net::ServerSocket;


MyActivity::MyActivity(const ServerSocket& so):
    _activity(this, &MyActivity::runActivity),
    _socket(so)
{
//    memset(vis, 0, sizeof(vis));
}

void MyActivity::start()
{
    _activity.start();
}

void MyActivity::stop()
{
    _activity.stop();
    _activity.wait();
}

void MyActivity::runActivity()
{

    while(!_activity.isStopped())
    {
        std::cout << "Server : Activity find is running: " << std::endl;

        //if client
        try
        {
            if( _socket.poll(Poco::Timespan(TIME,0), Poco::Net::Socket::SelectMode::SELECT_READ))
            {
                Poco::Net::StreamSocket ss = _socket.acceptConnection();
                ss.setNoDelay(true);

                runnable._queue.push(ss);
                pool.addCapacity(1);
                pool.start(runnable);
                std::cout << "thread" << std::endl;
             }
        }
        catch(Poco::ExistsException& e)
        {
            std::cout << "Server : error: "<< e.what() << std::endl;
        }

    }
}
*/

/*
//if client
try
{
    if( _socket.poll(Poco::Timespan(TIME,0), Poco::Net::Socket::SelectMode::SELECT_READ))
    {
        Poco::Net::StreamSocket ss = _socket.acceptConnection();
        ss.setNoDelay(true);

        runnable._queue.push(ss);

        Poco::ThreadPool::defaultPool().start(runnable);
        std::cout << "thread" << std::endl;
        //Poco::Util::ServerApplication::aitForTerminationRequest();

     }
}
catch(Poco::ExistsException& e)
{
    std::cout << "Server : error: "<< e.what() << std::endl;
}
*/

/*

                ThreadRunnable runnable[1024];
                for(int i=0; i<MAX_THREAD; i++)
                {
                    if(!vis[i])
                    {
                        while(_queue.empty())
                        {
                            _queue.front();
                        }
                        Poco::ThreadPool::defaultPool().start(runnable[i]);

                        vis[i] = 1;
                    }
                }
*/


