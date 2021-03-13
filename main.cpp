#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include "helpers.h"

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

        void join(const char *room_num)
        {
            char cmd[] = ".join ";
            snd_server_msg(cmd);
            snd_server_msg(room_num);
            snd_server_msg("\n");
            rcv_server_msg();
            printf("%s", buffer);
        }

        void create()
        {
            char cmd[] = ".create\n";
            snd_server_msg(cmd);
            rcv_server_msg();
            printf("%s", buffer);
        }

        void snd_server_msg(const char *msg)
        {
            dprintf(sockfd, "%s", msg);
        }

        void rcv_server_msg()
        {
            int rc = read(sockfd, buffer, sizeof(buffer));
            buffer[rc] = '\0';
        }

};

Robot::Robot(char *ip_addr, char *port)
{
        char name[] = "robot\n";
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(str_to_int(port));
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
            exit_with_perror(sock_error);
        if (!inet_aton(ip_addr, &(addr.sin_addr)))
            exit_with_perror(invalid_ip);
        if (connect(sockfd, (struct sockaddr *) &addr, sizeof(addr)))
            exit_with_perror(connect_error);
        snd_server_msg(name);
        rcv_server_msg();
        printf("%s", buffer);
}

int main(int argc, char *argv[])
{
    int next_action = check_params(argc, argv);
    if (next_action == quit)
    {
        printf("%s %s\n", invalid_params, usage);
        return 1;
    }
    Robot r(argv[1], argv[2]);
    if (next_action == join)
        r.join(argv[4]);
    else if (next_action == create)
        r.create();
    return 0;
}
