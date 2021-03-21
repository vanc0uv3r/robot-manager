#ifndef ROBOT_MANAGER_COMPETITOR_H
#define ROBOT_MANAGER_COMPETITOR_H
#include <stdlib.h>
#include <stdio.h>

class Competitor
{
    char *nick;
    int raw;
    int prod;
    int money;
    int plants;
    int auto_plants;

public:
    Competitor()
    {nick = (char *) malloc(sizeof(*nick) * 32);}

    char *get_nick() {return nick;}

    ~Competitor() {free(nick);}

    void update_fields(char *line);

    void print_fields();
};



#endif //ROBOT_MANAGER_COMPETITOR_H
