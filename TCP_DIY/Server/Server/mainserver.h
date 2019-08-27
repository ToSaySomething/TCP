#ifndef MAINSERVER_H
#define MAINSERVER_H

#include "activity.h"
#include "_appconnectionfactory.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Application.h"
#include "Poco/Timestamp.h"

using Poco::Util::ServerApplication;
using Poco::Util::Application;

class CustomTCPServer : public ServerApplication
{
public:
    CustomTCPServer() {}
    ~CustomTCPServer() {}
protected:
    void initialize(Application& self);
    void uninitialize();
    int main(const std::vector<std::string>& args);
};

#endif // MAINSERVER_H
