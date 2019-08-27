//client
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/StreamCopier.h"

#include "myclient.h"
#include "mainwindow.h"
#include <iostream>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

/*
void fun(){
    //connect
    const char* ipaddr = "127.0.0.1"; // the server address.
    Poco::Net::SocketAddress sa(ipaddr, 9000); // the server port.
    Poco::Net::StreamSocket socket(sa);

    //send
    std::string strsend = {"Poco send test"};
    int len = socket.sendBytes(strsend.c_str(), strsend.length());
    if(len < 0){
        std::cout << "send write error" << std::endl;
    }

    Poco::Net::SocketStream str(socket);
    socket.shutdownSend();
    // Writes all bytes readable from str into std::cout, using an internal buffer.
    Poco::StreamCopier::copyStream(str, std::cout);
}
*/
