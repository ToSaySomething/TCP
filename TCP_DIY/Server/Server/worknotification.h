#ifndef WORKNOTIFICATION_H
#define WORKNOTIFICATION_H

#include "Poco/Notification.h"
#include "Poco/NotificationQueue.h"
#include "Poco/Net/StreamSocket.h"

using Poco::Notification;
using Poco::NotificationQueue;
using Poco::Net::StreamSocket;

class WorkNotification: public Notification
{
public:
    WorkNotification(StreamSocket so ): _so(so) {}
    StreamSocket  socket() const
    {
        return _so;
    }
private:
    StreamSocket _so;
};

#endif // WORKNOTIFICATION_H
