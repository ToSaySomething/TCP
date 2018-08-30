#include "Poco/Data/Common.h"
#include "Poco/Data/SQLite/Connector.h"
#include <iostream>

using namespace Poco::Data;

void init(){

    SQLite::Connector::registerConnector();

}

void shutdown(){

    SQLite::Connector::unregisterConnector();
}

int main(int argc, char* argv[]){

    init();
    Session ses("SQLite", "sample.db");
    int count = 0;
    ses << "SELECT COUNT(*) FROM PERSON", into(count), now;
    std::cout << "People in DB " << count;
    shutdown();
    return 0;
}

