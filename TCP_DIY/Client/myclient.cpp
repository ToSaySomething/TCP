#include "myclient.h"
#include "config.h"
#include "people.pb.h"
#include "people.pb.cc"

#include <iostream>
#include <string>

MyClient::MyClient(): _socket (nullptr),_ipaddr(""),_port(0)
{
}

void MyClient::myconnect(std::string ip, const int port)
{
    setip(ip);
    setport(port);
    SocketAddress sa(_ipaddr, _port); // the server port.
    _socket = new Poco::Net::StreamSocket (sa);
    receive();
}

void MyClient::close()
{
    _socket->close();
    std::cout << " client end " << std::endl;
}

QString MyClient::send(std::string strsend)
{//send

    int len = _socket->sendBytes(strsend.c_str(), strsend.length());
    if(len < 0)
    {
        std::cout << "client: send error" << std::endl;
    }

    return receive();
}

QString MyClient::receive()
{//receive
    Poco::Thread::sleep(TIME);
    char rece[1024]={""};
    int len1;
    if(_socket->available())
    {
        len1 = _socket->receiveBytes(rece, 1024);
    }
    if(len1 == 0)
    {
        std::cout << "client: receive error" << std::endl;
        return NULL;
    }

    std::cout << "client receive: \n" << rece << std::endl;

    QString reQString = QString::fromStdString(rece);
    return reQString;
}

void MyClient::send_heardbeat()
{
   // Poco::Timespan stimeout( TIME );
    //_socket->setReceiveTimeout(stimeout);
    std::string strcontext = package();
    send_context(strcontext);
}
// head name id email
bool MyClient::send_context(std::string strsend)
{
    //Poco::Timespan stimeout( TIME );
    //_socket->setSendTimeout(stimeout);
    int len = _socket->sendBytes(strsend.c_str(), strsend.length());
    if(len < 0)
    {
        //std::cout << "client: send error" << std::endl;
        return false;
    }
    else
    {
        //std::cout << "client: send somethings" << std::endl;
        return true;
    }
}

std::string MyClient::package()
{
    std::string data1;
    people::People p;
    p.set_head("head");
    p.set_name("Hideto");
    p.set_id("131232");
    p.set_email("hideto.bj@gmail.com");
    p.SerializeToString(&data1);
    return data1;
}

std::string MyClient::display_heartbeat(std::string rece_string)
{
    std::string data = rece_string;
    people::People p;
    p.ParseFromString(data);

    std::cout << "People: " << std::endl;
    std::cout << "Head: " << p.head() << std::endl;
    std::cout << "Name: " << p.name() << std::endl;
    std::cout << "ID: " << p.id() << std::endl;
    std::cout << "Email: " << p.email() << std::endl;
    return data;
}
