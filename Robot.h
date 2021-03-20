#ifndef ROBOT_MANAGER_ROBOT_H
#define ROBOT_MANAGER_ROBOT_H

#include "stdio.h"
#include "helpers.h"
#include "unistd.h"
#include "stdlib.h"
#include "string.h"

struct market_info
{
    int raw;
    int min_price;
    int prod;
    int max_price;
};

struct service
{
    char *ip;
    int port;
    int sockfd;
};

class Robot
{
private:
    market_info market;
    service s;
    char *nick;
    char *buffer;
    char *line;
    int raw;
    int prod;
    int money;
    int plants;
    int auto_plants;
    int buffer_p;
    int buffer_size;
    void rcv_server_msg();

    void parse();

    void resize_buffer();

    void resize_line();

    void connect_serv();

    void enter_server();

    void buffer_shift(int cmd_pos);

    int need_realloc() {return buffer_size - buffer_p < read_size;}

public:
    Robot(char* serv_ip, char *serv_port, char *nickname);

    ~Robot();

    void join(const char *room_num);

    void create(char *players);

    void wait_for_start();

    void get_info();

    void get_market();

    int check_end();
    
    void make_turn();

    void sell();

    void buy();

    void make_prod();

    void check_read(int rc);

    void me();

    void wait_other();

    int no_winner();

    int define_winner();

    void print_buffer() {printf("%s\n", line);}

};

#endif //ROBOT_MANAGER_ROBOT_H
