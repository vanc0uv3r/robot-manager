#include "Robot.h"
#include <sys/socket.h>
#include <arpa/inet.h>

Robot::Robot(char *ip_addr, char *port)
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
    snd_server_msg("robot\n");
    rcv_server_msg();
    print_buffer();
}

void Robot::join(const char *room_num)
{
    snd_server_msg(".join ");
    snd_server_msg(room_num);
    snd_server_msg("\n");
    rcv_server_msg();
    print_buffer();
}

void Robot::create()
{
    snd_server_msg(".create\n");
    rcv_server_msg();
    print_buffer();
}

void Robot::rcv_server_msg()
{
    int rc = read(sockfd, buffer, sizeof(buffer) - 1);
    buffer[rc] = '\0';
}

char *Robot::parse_str(int &i)
{
    int j = 0;
    char *num = (char *)malloc(13);
    while (buffer[i] > '9' || buffer[i] < '0')
        i++;
    while (buffer[i] != ' ' && buffer[i] != '\n')
    {
        num[j] = buffer[i];
        j++;
        i++;
    }
    num[j] = '\0';
    return num;
}

void Robot::parse(int **params, int params_num)
{
    int k, i = 0;
    for (k = 0; k < params_num; ++k)
    {
        char *num = parse_str(i);
        *params[k] = str_to_int(num);
        printf("%d\n", *params[k]);
    }
}