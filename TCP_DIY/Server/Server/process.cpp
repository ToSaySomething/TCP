#include "process.h"

#define _PATH_ "./tfifo"
const int _SIZE = 100;
Processrun::Processrun()
{
    sql.create();
}

std::string Processrun::_run(std::string str)
{
    //net -> sql
    s_send = str;
    pid_t child = fork();
    std::string s = "";

    if(child)
    {
        parentdo();
    }
    else
    {
        s = childdo();
    }
    unlink(_PATH_);
    return s;
}

void Processrun::parentdo()
{
    printf("---do write---\n");
    int ret = mkfifo(_PATH_,0666|S_IFIFO);
    if(ret == -1)
    {
        perror("create error\n");
        return;
    }
    int fd = open(_PATH_, O_WRONLY);
    if(fd < 0)
    {
        perror("open error\n");
        return ;
    }
    char buff[_SIZE];
    memset(buff, '\0', sizeof(buff));

    std::string s = sqldo(s_send);
    s.copy(buff, s.length(), 0);
    buff[s.length()] = '\0';

    int ret1 = write(fd, buff, strlen(buff)-1);

    if( ret1 < 0 )
    {
        perror("write error!\n");
    }

    close(fd);
}

std::string  Processrun::childdo()
{
    printf("---do read---\n");
    std::string result = "";
    int fd = open(_PATH_,O_RDONLY);
    if(fd < 0)
    {
        perror("open file error!\n");
        return result;
    }
    char buf[_SIZE];
    memset(buf,'\0',sizeof(buf));

    int ret = read(fd, buf, sizeof(buf));
    if(ret <= 0)
    {
        perror("read end or error!\n");
    }
    result = buf;

    close(fd);
    return result;

}

Person Processrun::tra(people::People p)
{
    Person person;
    person.name = p.name();
    person.id = p.id();
    person.email = p.email();
    return person;
}

std::string Processrun::sqldo(std::string rece_string)
{
    std::string sum ="";
    std::string data = rece_string;
    people::People p;
    p.ParseFromString(data);
    Person person = tra(p);
    int b = atoi(p.head().c_str());
    struct Person person_[50];
    switch (b) {
    case 1://insert
        sum += sql.insert(person);
        break;
    case 2://delete
        sum += sql.del(person);
        break;
    case 3://update
        sum += sql.update(person);
        break;
    case 4://select
        sum += sql.select(person_);
        break;
    case 5://select
        sum += sql.display(person_);
        break;
    }
    //sql.exit();
    std::cout<< "Sql: " << sum << std::endl;
    return sum;
}
