#ifndef PROCESS_H
#define PROCESS_H

#include "mysql.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include "people.pb.h"

class  Processrun
{

public:
     Processrun();

     ~Processrun(){}

     std::string _run(std::string str);
     std::string childdo();
     void parentdo();
     std::string sqldo(std::string rece_string);
     Person tra(people::People p);
private:
     std::string s_send;
     std::string s_rece;
     MYSQL sql;
};

#endif // PROCESS_H
