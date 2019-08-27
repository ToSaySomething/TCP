/*
 * #include "thread.h"
#include <iostream>
#include <thread>
ThreadRunnable::ThreadRunnable()

{
    while(!_queue.empty()) _queue.pop();
}

void ThreadRunnable::run()
{
    std::cout<< std::this_thread::get_id() << std::endl;

    if(!_queue.empty())
    {

            std::cout<<"Server: run  "<<_queue.front().peerAddress() <<std::endl;
            Poco::Net::StreamSocket so = _queue.front();
            Poco::Thread thread;
            thread.sleep(1000);

            so.setReceiveTimeout(TIME);
            char buff[MAX_LEN]={0};
            int len = so.receiveBytes(buff, MAX_LEN);

            while(true)
            {
                if(len <= 0)
                {
                    std::cout<< "Server: no receives: " << std::endl;
                     _queue.pop();
                    break;
                }
                else
                {
                    std::cout<< "Server: Sucessful receive: " << buff << " from client " << _queue.front().peerAddress() <<std::endl;
                    char data[1024] = {"hello from server\n"};
                    so.sendBytes(data, sizeof(data));
                }
                so.setReceiveTimeout(TIME);
                len = so.receiveBytes(buff, MAX_LEN);
            }




    }
}
*/
