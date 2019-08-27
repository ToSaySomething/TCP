#ifndef TIMER_H
#define TIMER_H

#include "Poco/Timer.h"
#include <iostream>
#include <string>

using Poco::Timer;         // 使用Timer基类的成员
using Poco::TimerCallback; // 使用TimerCallback基类的成员

class StatTimer
{
public:
    Timer timer;
    TimerCallback<StatTimer> callback;  // 套用模板，将StatTimer类关联起来
    bool end;

    StatTimer();
    bool start();
    void onTimer(Timer& t);
    void stop();
    void settime(size_t cnt);//change timer

private:
    size_t _counter = 0;
};

#endif // TIMER_H
