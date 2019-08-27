#include "people.pb.h"
#include "people.pb.cc"
#include "_appconnection.h"


Connection1::Connection1(const StreamSocket& s):
    TCPServerConnection(s), _state(ConnectionState::None),
    _socket(const_cast<StreamSocket&>(s)),
    _buff(new char[MAX_RECV_LEN]),
    _buff1(new char[MAX_RECV_LEN])
{
    //sql.create();
}

Connection1::~Connection1()
{
}

bool Connection1::onlyread()// read
{
    int len = _socket.receiveBytes(_buff, MAX_RECV_LEN);
    if(!len)
    {
        shutdown(CLIENT_CLOSE);// receive nothing
        return false;//closed socket
    }
    else
    {
        //format is ill?
        bool flag = isflag(_buff, len);
        if(!flag)
        {
            std::cout << "ill format" <<std::endl;
            shutdown(ILLED_FORMAT);
            return false;
        }
        std::cout<< "Server: Sucessful receive: " << _buff << " from client: "<< _socket.peerAddress() <<std::endl;

        //send to client
        std::string data ="server received" ;
//        data += tra_message(data);
        _socket.sendBytes(data.data(), MAX_RECV_LEN, 0);
        return true;
    }
}

bool Connection1::isflag(const char* buffer, const int len)
{
    if(len >= MIN_RECV_LEN)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Connection1::run()
{
    Poco::Util::Application& app = Poco::Util::Application::instance();
    // 日志输出连接的TCP用户的地址（IP和端口）
    app.logger().information( "Request from " + _socket.peerAddress().toString() );
    _state = Connection1::Established;//connect
    try
    {
        while(true)
        {
            if(_state == Connection1::Established)
            {
                bool flag = _socket.poll(Poco::Timespan(TIME,0), Poco::Net::Socket::SelectMode::SELECT_READ);
                if(flag)
                {
                    if(!read())
                    {
                        break;
                    }
                }
            }
            else if(_state == Connection1::Closed)
            {
                    return;
            }
        }
    }
    catch (Poco::Exception& e)
    {
        app.logger().log(e);
    }
    app.logger().information("Server: the connection closed.");
}

bool Connection1::read()
{
    bool flag = true;
    StatTimer *timer = new StatTimer();
    timer->start();
    while(true)
    {
        if(readheardbeat())
        {   //update timer
           timer->settime(0);
           flag = true;
        }
        else
        {
            flag = false;
        }

        if (timer->end)
        {
            timer->stop();
            flag = false;
            break;
        }
    }
    return flag;
}

bool Connection1::readheardbeat()
{
    if(!_socket.available())   return false;

//    Poco::Timespan stimeout( TIME );
//    _socket.setReceiveTimeout(stimeout);
    int len = _socket.receiveBytes(_buff, MAX_RECV_LEN,0);
    _buff[len] = '\0';

    if(0 >= len)
    {
        shutdown(CLIENT_CLOSE);// receive nothing
        return false;//closed socket
    }
    else
    {   //format
        bool flag = isflagformat(_buff, len);
        if(!flag)
        {
            std::cout << "*-ill format" <<std::endl;
            shutdown(ILLED_FORMAT);
            return false;
        }

//        display_message(_buff);

        return true;
    }
}

bool Connection1::isflagformat(std::string buffer, const int len)
{
    if(len >= MIN_RECV_LEN)
    {
        std::string data = buffer;
        people::People p;
        p.ParseFromString(data);

        if(p.head() == "head" )
        {
            std::cout<< "Server: receive heartbeats  from client " << _socket.peerAddress() <<std::endl;
        }
        else
        {
            std::cout<< "Server: receive " << buffer << " from client " << _socket.peerAddress() <<std::endl;
            std::string send = process._run(buffer);
            // std::string send = sqldo(buffer);
            if(!send.empty())
            {

                _socket.sendBytes(send.data(), MAX_RECV_LEN, 0);
            }
            std::cout<< "Server send: \n" << send << std::endl;
        }
        return true;
    }
    else
    {
        return false;
    }
}

void Connection1::display_message(std::string rece_string)
{
    std::string data = rece_string;
    people::People p;
    p.ParseFromString(data);
    std::cout << "----------------------- " << std::endl;
    std::cout << "| People: " << std::endl;
    std::cout << "| Head: " << p.head() << std::endl;
    std::cout << "| Name: " << p.name() << std::endl;
    std::cout << "| ID: " << p.id() << std::endl;
    std::cout << "| Email: " << p.email() << std::endl;
     std::cout << "---------------------- " << std::endl;
}

std::string Connection1::tra_message(std::string data1)
{
    people::People p;
    p.set_head("head");
    p.set_name("Hideto");
    p.set_id("131232");
    p.set_email("hideto.bj@gmail.com");
    p.SerializeToString(&data1);
    return data1;
}

void Connection1::shutdown(const ShutdownReason& reason)
{
    if(_state == ClosedWait) return ;
    switch( reason )
    {
        case CLIENT_CLOSE:std::cout<< " closed client socket "<<std::endl;
            break;
        case SERVICE_CLOSE:std::cout<< " closed service socket "<<std::endl;
            break;
        case ILLED_FORMAT :std::cout<< "service :ill format "<<std::endl;
            break;
        default:break;
    }
    if(_state == Established)
    {
        _socket.shutdown();
    }
    _state = ClosedWait;
}

void Connection1::close(const ShutdownReason& reason)
{
    shutdown(reason);
}



/*
 * sql do
Person Connection1::tra(people::People p)
{
    Person person;
    person.name = p.name();
    person.id = p.id();
    person.email = p.email();
    return person;
}

std::string Connection1::sqldo(std::string rece_string)
{
    std::string sum ="";
    std::string data = rece_string;
    people::People p;
    p.ParseFromString(data);
    Person person = tra(p);
    int b = atoi(p.head().c_str());
    struct Person person_[50];
    switch (b) {
    case 1://insert
        sum += sql.insert(person);
        break;
    case 2://delete
        //person = {"li","21","21221"};
        sum += sql.del(person);
        break;
    case 3://update
        //person = {"li","21","21221"};
        sum += sql.update(person);
        break;
    case 4://select
        sum += sql.select(person_);
        break;
    case 5://select
        sum += sql.display(person_);
        break;
    }
    //sql.exit();
    std::cout<< "Sql: " << sum << std::endl;
    return sum;
}*/


/*
bool CustomTCPConnection::read()
{
    onlyread();
    TimerExample example;

    Timer timer(0, 500);
    timer.start(TimerCallback<TimerExample>(example, &TimerExample::onTimer));

    while(true)
    {
        if(readheardbeat())
        {//update timer
           timer.start(TimerCallback<TimerExample>(example, &TimerExample::onTimer));
        }else{
            return false;
        }
    }
    return true;
}
*/



