#ifndef ROBOT_MANAGER_ROBOT_H
#define ROBOT_MANAGER_ROBOT_H

#include "stdio.h"
#include "helpers.h"
#include "unistd.h"

class Robot
{
private:
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

    void rcv_server_msg();

};

#endif //ROBOT_MANAGER_ROBOT_H
