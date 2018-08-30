#include "mysql.h"
#include <vector>
#include <iostream>
#include <sstream>

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;
using Poco::Data::SQLite::Connector;


void MYSQL::init()
{
    Connector::registerConnector();
}

void MYSQL::exit()
{
     Connector::unregisterConnector();
}

void MYSQL::create()
{
    init();
    Session session = getsession();
    session << "DROP TABLE IF EXISTS Person", now;//-drop
    session << "CREATE TABLE Person (Name VARCHAR(20), ID VARCHAR(12), Email VARCHAR(12)) ", now;
    exit();
}
std::string MYSQL::insert(Person person)
{
    init();

    Session session = getsession();
    session << "INSERT INTO Person VALUES('" + person.name + "' ,'" + person.id + "' ,'" +person.email + "')",  now;
    std::string str = " Sucessful INSERT ";
    exit();
    return str;
}

std::string MYSQL::update(Person person)
{
    // name update id
    init();
    Session session = getsession();
    if(person.name!="" && person.id!="")
        session << "update Person set ID = ? WHERE Name= ?", use(person.id), use(person.name), now;
    if(person.name!="" && person.email!="")
        session << "update Person set Email = ? WHERE Name= ?", use(person.email), use(person.name), now;
    std::string str = " Sucessful update ";
    exit();
    return str;
}



std::string MYSQL::del(Person person)
{
    init();
    Session session = getsession();
    session << "DELETE FROM Person WHERE Name = ?", use(person.name), now;
    std::string str = " Sucessful DELETE ";
    exit();
    return str;
}

std::string MYSQL::select(Person person[])//cnt_max 50
{
    init();
    std::string str = "";
    Session session = getsession();
    typedef Poco::Tuple<std::string, std::string, std::string > _Person;
    typedef std::vector<_Person> People;
    People people;
    session << "SELECT Name, ID, Email FROM Person", into(people), now;
    str += "Name\tID\tEmail\n";
    int cnt = 0;
    for (People::const_iterator it = people.begin(); it != people.end(); ++it)
    {
        str += (it->get<0>()+"\t"+
                it->get<1>()+"\t"+
                it->get<2>()+"\n");
        person[cnt].name = it->get<0>();
        person[cnt].id = it->get<1>();
        person[cnt++].email = it->get<2>();
    }
    exit();
    return str;
}

std::string MYSQL::display(Person person[])
{
    init();
    std::string str = "";
    Session session = getsession();
    typedef Poco::Tuple<std::string, std::string, std::string > _Person;
    typedef std::vector<_Person> People;
    People people;
    session << "SELECT * FROM Person", into(people), now;
    str += "Name\tID\tEmail\n";
    int cnt = 0;
    for (People::const_iterator it = people.begin(); it != people.end(); ++it)
    {
        str += (it->get<0>()+"\t"+
                it->get<1>()+"\t"+
                it->get<2>()+"\n");
        person[cnt].name = it->get<0>();
        person[cnt].id = it->get<1>();
        person[cnt++].email = it->get<2>();
    }
    exit();
    return str;
}
