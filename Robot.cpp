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
    if (rc == -1)
        perror("Read error: ");
    else if (rc == 0)
    {
        printf("%s\n", server_dis);
        exit(0);
    }
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
    }
}

void Robot::sell()
{
    if (prod > 0)
    {
        snd_server_msg("sell ");
        snd_server_msg(int_to_str(prod));
        snd_server_msg(" ");
        snd_server_msg(int_to_str(m.max_price));
        snd_server_msg("\n");
        printf("Selling %d prods now with price %d\n", prod, m.max_price);
        rcv_server_msg();
    }
}

void Robot::buy()
{
    snd_server_msg("buy 2 ");
    snd_server_msg(int_to_str(m.min_price));
    snd_server_msg("\n");
    printf("Try to buy 2 raws which costs %d\n", m.min_price);
    rcv_server_msg();
}

void Robot::make_prod()
{
    if (raw > 0 && raw < 3)
    {
        snd_server_msg("prod ");
        snd_server_msg(int_to_str(raw));
        snd_server_msg("\n");
        printf("Making %d prod...\n", raw);
        rcv_server_msg();
    }
}

void Robot::me()
{
    printf("INFORMATION about me:\n");
    printf("My BALANCE is %d$\n", money);
    printf("I have %d RAWS\n", raw);
    printf("I have %d PROD\n", prod);
}

void Robot::wait_other()
{
    while (strstr(buffer, "Trading") == NULL)
        rcv_server_msg();
}

void Robot::parse_info()
{
    int *fields[5] = {&raw, &prod, &money, &plants, &auto_plants};
    parse(fields, 5);
}

void Robot::parse_market()
{
    int *fields[4] = {&m.raw, &m.min_price, &m.prod, &m.max_price};
    parse(fields, 4);
}

void Robot::make_turn()
{
    snd_server_msg("turn\n");
    rcv_server_msg();
}

void Robot::get_market()
{
    snd_server_msg("market\n");
    rcv_server_msg();
}

void Robot::get_info()
{
    snd_server_msg("info\n");
    rcv_server_msg();
}

void Robot::wait_for_start()
{
    while (strstr(buffer, "START") == NULL)
        rcv_server_msg();
}

void Robot::define_winner()
{
    if (strstr(buffer, "YOU_WIN") != NULL)
        printf("YOO! I win!\n");
    else
        printf("DAMN! I lose :((\n");
}
