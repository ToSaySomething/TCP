

# POCO::NET::TCPServer框架学习以及使用

## 一、预备知识：

### 1、POCO

​		POCO C++ Libraries是一套第三方库，去年的这个时候才进入公司实习，前辈让我接触这个库很难受官网全是英文:https://pocoproject.org/，GitHub：https://github.com/pocoproject/poco ，库方法查找：https://pocoproject.org/docs/99150-GMakeBuildNotes.html

​		开始做数据库，使用的是POCO::DATA::SQLite（当时设备使用的数据库去存储的比较少，最多把人脸特征下载到设备做个比对的小库，相对来说，用HTTP去Post引擎的大库去找和相机的小库去找小库更加有效率，但是由于相机的小库没有这么大，最多几k，所以场景适用在人数比较少而效率要求特别高的情况下）
​		修改一下，SQLite是一个开源的嵌入式关系数据库，实现自包容、零配置、支持事务的SQL数据库引擎。 

​		前辈让我啃它的NET::TCP贼难受，网上讲解的只有两三篇，官网全是英文，有一些例子关于NET库里面的Http和Mail等等https://github.com/pocoproject/poco/tree/master/Net/samples

​		当时是在虚拟的Linux下做的一套TCP客户端和服务端的通信，当时参考了两篇大佬的blog（非常经典）：

​				http://www.cppblog.com/richbirdandy/archive/2010/09/10/123994.html

​				https://www.cnblogs.com/bleachli/p/4352959.html

### 2、长连接和短连接

![image](https://github.com/ToSaySomething/TCP/raw/master/Pic/01.png)

### 3、设计模式：工厂模式

​		

## 二、POCO::NET::TCPServer框架使用

### 1、ProtoBuffer 协议文件

协议![image](https://github.com/ToSaySomething/TCP/raw/master/Pic/02.png)

```protobuf
message People 
{  
  required string head = 1; 
  required string name = 2;  
  required string id = 3;  
  required string email = 4;  
}  
```



### 2、分析

具体文件：

![image](https://github.com/ToSaySomething/TCP/raw/master/Pic/03.png)

#### 2.1 先看Main.cpp

```C++
int main(int argc, char *argv[])
{
    return CustomTCPServer().run(argc, argv);;
}
```

#### 2.2 CustomTCPServer类

```C++
#include "activity.h" //继承了runnable 可以理解成开线程 其中的run方法就是线程的入口
#include "_appconnectionfactory.h" //继承了TCPServerConnection
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Application.h"
#include "Poco/Timestamp.h"
```

主要是调用select 模型，处理连接收发消息

```C++
    Poco::Net::ServerSocket serverScoket ( 9000 );
   
    Poco::Timespan timeout( 1000, 0 ); //设置超时 timeout
    serverScoket.setReceiveTimeout( timeout );

    Params::Ptr params = new Params;
    params->setMaxQueued( 1020 );//设定队列的最大长度
    params->setMaxThreads( 1020 );//设定　TcpServer 最大线程的个数

    MyActivity server(new MyConnectionFactory(),serverScoket, params );//a thread
    //TCPServer server(serverScoket, params);
    server.start();
    waitForTerminationRequest();
    server.stop();

    return Application::EXIT_OK;
```

#### 2.3  MyActivity类

```C++
#include "work.h" //调用了Worker类里面的消息队列
#include "params.h"
/*调用了Params类的自己写的多线程池[主要是设置参数大小]（主要调用了优先队列 Poco::Thread::PRIO_NORMAL）*/
```

```C++
void MyActivity::start()
{
    _activity.start();
}
```

直接运行了runActivity()方法

```C++
bool flag = _socket.poll(timeout, Poco::Net::Socket::SELECT_READ);//select的poll
if (flag)
{
    try
    {
        StreamSocket ss = _socket.acceptConnection();//建立连接
        ss.setNoDelay(true);
        //queue.enqueueNotification(new WorkNotification(ss));
        _worker->enqueue(ss);//入队，实际处理还是worker

    }
    catch(Poco::Exception& e)
    {
        std::cout<< " Exception"<< e.what() << std::endl;
    }
}
```

#### 2.4 然后看worker类

```C++
void Worker::enqueue(const StreamSocket& ss)
{
    Poco::FastMutex::ScopedLock lock(_mutex);
    try
    {
        _queue.enqueueNotification(new WorkNotification(ss));
        _threadPool.start(*this);
        _curthreads++;
    }
    catch(Poco::Exception&e)
    {
    }
}
```

其主要还是把我们入队的那个消息交给了线程池，线程池分出来了一个空闲的线程去处理的

```C++
void Worker::run()
{
    AutoPtr<Worker> guard(this, true);//keep alive

    while (true)
    {
        AutoPtr<Notification> pNf(_queue.waitDequeueNotification());
        if(pNf)
        {
            WorkNotification* pWorkNf = dynamic_cast<WorkNotification*>(pNf.get()); //把入队的pNf 转换成 WorkNotification 类型的对象
            if (pWorkNf)
            {
                Connection1 connect(pWorkNf->socket());//TCPServerConnection
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
```

#### 2.5 然后看TCPServerConnection

​		TCPServerConnection原来还是继承了Runnable，有兴趣的话可以下载源码看一下TCPServerConnection接口

```C++
class Net_API TCPServerConnection: public Poco::Runnable
```

​		其实我写的Connection1 继承了的TCPServerConnection类，其实是在TCPServerConnection上加了一层。定义了一些常量和状态枚举 ，其实我只是想做一层有心跳包的长连接，同时可能有收到有协议发过来的数据库操作（主要的还有run()里面判断状态去读取数据的，代码比较多，一是我当时刚刚写这种代码，二是判断协议的内容比较多）

### 3、题外补充 进程通信参考process.h

​		原理：父子进程管道fork()
