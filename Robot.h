#ifndef ROBOT_MANAGER_ROBOT_H
#define ROBOT_MANAGER_ROBOT_H

#include "stdio.h"
#include "helpers.h"
#include "unistd.h"
#include "stdlib.h"
#include "string.h"
#include "Competitor.h"

struct market_info
{
    int raw;
    int min_price;
    int prod;
    int max_price;
};

struct service_info
{
    char *ip;
    int port;
    int sockfd;
};

struct resources
{
    int raw;
    int prod;
    int money;
    int plants;
    int auto_plants;
};

struct object_list
{
    Competitor *c;
    object_list *next;
};

class Robot
{
private:
    market_info market;
    service_info s;
    resources own;
    object_list *competitors;
    char *nick;
    char *buffer;
    char *line;
    int buffer_p;
    int buffer_size;

    void rcv_server_msg();

    void parse();

    void resize_buffer();

    void resize_line();

    void connect_serv();

    void enter_server();

    void buffer_shift(int cmd_pos);

    void update_competitor();

    void add_competitor(object_list **l);

    void clear_competitors();

    int need_realloc() {return buffer_size - buffer_p < read_size;}

    int look_for_winner();

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

    void get_competitors();

    int no_winner();

    int define_winner();

    void print_buffer() {printf("%s\n", line);}

};

#endif //ROBOT_MANAGER_ROBOT_H
