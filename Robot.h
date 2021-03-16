#ifndef ROBOT_MANAGER_ROBOT_H
#define ROBOT_MANAGER_ROBOT_H

#include "stdio.h"
#include "helpers.h"
#include "unistd.h"
#include "stdlib.h"
#include "string.h"

struct market
{
    int raw;
    int min_price;
    int prod;
    int max_price;
};

class Robot
{
private:
    market m;
    char *nick;
    int raw;
    int prod;
    int money;
    int plants;
    int auto_plants;
    int sockfd;
    char buffer[BUFFER_SIZE];

    void snd_server_msg(const char *msg) {dprintf(sockfd, "%s", msg);}

    void rcv_server_msg();

    void parse(int *params[], int params_num);

    char *parse_str(char **start, int& i);

public:
    Robot(char *nickname, char* ip_addr, char *port);

    ~Robot();

    void join(const char *room_num);

    void create(char *players);

    void wait_for_start();

    void get_info();

    void get_market();

    void make_turn();

    void sell();

    void buy();

    void make_prod();

    void me();

    void parse_market();

    void parse_info();

    void wait_other();

    int no_winner() {return strstr(buffer, "WIN") == NULL;}

    void define_winner();

    void print_buffer() {printf("%s\n", buffer);}

};

#endif //ROBOT_MANAGER_ROBOT_H
