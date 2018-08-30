#ifndef MYSQL_H
#define MYSQL_H

#include "Poco/Data/Session.h"
#include "Poco/Data/SQLite/Connector.h"

using Poco::Data::Session;
using Poco::Data::Statement;
using Poco::Data::SQLite::Connector;

struct Person
{
    std::string name;
    std::string id;
    std::string email;
};

class MYSQL
{

public:
    MYSQL(){}
    ~MYSQL(){}

public:
    //operator
    void init();
    void exit();
    void create();
    std::string  insert(Person person);
    std::string update(Person person);
    std::string del(Person person);
    std::string select(Person person[]);
    std::string display(struct Person person[]);

private:
    static Session getsession(){
        return Session(Connector::KEY, "Sample.db");
    }

};//class MYSQL

#endif // MYSQL_H
