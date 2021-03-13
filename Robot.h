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
    int raw;
    int prod;
    int money;
    int plants;
    int auto_plants;
    int sockfd;
    char buffer[BUFFER_SIZE];
public:
    Robot(char* ip_addr, char *port);

    ~Robot()
    {
        close(sockfd);
    };

    void join(const char *room_num);

    void create();

    void snd_server_msg(const char *msg)
    {
        dprintf(sockfd, "%s", msg);
    }

    void print_buffer()
    {
        printf("%s\n", buffer);
    }

    void rcv_server_msg();

    void wait_for_start()
    {
        while (strncmp("START", buffer + 2, 5) != 0)
            rcv_server_msg();
    }

    void get_info()
    {
        snd_server_msg("info\n");
        rcv_server_msg();
    }

    void get_market()
    {
        snd_server_msg("market\n");
        rcv_server_msg();
    }

    void parse_market()
    {
        int *fields[4] = {&m.raw, &m.min_price, &m.prod, &m.max_price};
        parse(fields, 4);
    }

    void parse_info()
    {
        int *fields[5] = {&raw, &prod, &money, &plants, &auto_plants};
        parse(fields, 5);
    }

    void parse(int *params[], int params_num);

    char *parse_str(int& i);

};

#endif //ROBOT_MANAGER_ROBOT_H
