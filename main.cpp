#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include "helpers.h"

class Robot
{
    private:
        int sockfd;
    public:
        Robot(char* ip_addr, char *port)
        {
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(str_to_int(port));
            if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
                exit_with_perror(sock_error);
            if (!inet_aton(ip_addr, &(addr.sin_addr)))
                exit_with_perror(invalid_ip);
            if (connect(sockfd, (struct sockaddr *) &addr, sizeof(addr)))
                exit_with_perror(connect_error);
        }
};

int main(int argc, char *argv[])
{
    check_params_num(argc);
    Robot r(argv[1], argv[2]);
    return 0;
}
