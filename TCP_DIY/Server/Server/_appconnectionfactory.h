#ifndef _APPCONNECTIONFACTORY_H
#define _APPCONNECTIONFACTORY_H
#include "_appconnection.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/Net/StreamSocket.h"

using Poco::Net::TCPServerConnection;
using Poco::Net::TCPServerConnectionFactory;

using Poco::Net::StreamSocket;

class MyConnectionFactory: public TCPServerConnectionFactory
{
public:
    TCPServerConnection* createConnection(const StreamSocket& socket);
};


#endif // _APPCONNECTIONFACTORY_H
