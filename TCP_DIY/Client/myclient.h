#ifndef MYCLIENT_H
#define MYCLIENT_H

#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/StreamCopier.h"
#include "Poco/Thread.h"
#include "people.pb.h"

#include <QMainWindow>
#include <iostream>

using Poco::Net::StreamSocket;
using Poco::Net::SocketAddress;

class MyClient
{
    const long long TIME = 1000;

public:
    MyClient();
    ~MyClient(){}

public:
    void myconnect(std::string, const int port);
    QString send(std::string strsend);
    QString receive();

    void close();
    void setip(std::string ip)
    {
        _ipaddr = ip;
    }
    void setport(int port)
    {
        _port = port;
    }

public:
    void send_heardbeat();
    bool send_context(std::string strsend);
    std::string package();
    std::string display_heartbeat(std::string rece_string);

private:
    StreamSocket* _socket;
    std::string _ipaddr;
    int _port;

};

#endif // MYCLIENT_H
