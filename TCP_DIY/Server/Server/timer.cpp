#include "timer.h"

StatTimer::StatTimer() : timer(0, 1000), callback(*this, &StatTimer::onTimer), end(false)
{
       // _counter = 0;
}
// timer(0, 2000)，第一个参数默认设置为0,2000代表时间间隔为2秒
// callback的第二个参数指定定时器需要做的具体事情

bool StatTimer::start()
{
    try
    {
        timer.start(callback);   // 启动定时器线程

        //started = true;
    }
    catch (...)
    {
        return false;
        end = true;
    }

    return true;
}

void StatTimer::onTimer(Timer&/*t*/)
{

    ++_counter;
    //std::cout << "onTimer:"<< _counter << std::endl;
    if (_counter >= 10 )
    {
        end = true;
    }
}

void StatTimer::settime(size_t cnt)
{
    _counter = cnt;
}

void StatTimer::stop()
{
    timer.stop();  // 终止定时器线程
    std::cout << "Timer ended "<< _counter << std::endl;
}



