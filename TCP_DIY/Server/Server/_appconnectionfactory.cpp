#include "_appconnectionfactory.h"

TCPServerConnection* MyConnectionFactory::createConnection(const StreamSocket& socket)
{
    return new Connection1(socket);
}
