#include "Robot.h"
#include <sys/socket.h>
#include <arpa/inet.h>

Robot::Robot(char *nickname, char *ip_addr, char *port)
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
    nick = (char *)malloc(strlen(nickname) * sizeof(*nick) + 1);
    strncpy(nick, nickname, strlen(nickname) + 1);
    rcv_server_msg();
    snd_server_msg(nickname);
    snd_server_msg("\n");
    rcv_server_msg();
    print_buffer();
}

Robot::~Robot()
{
    free(nick);
    close(sockfd);
}

void Robot::join(const char *room_num)
{
    snd_server_msg(".join ");
    snd_server_msg(room_num);
    snd_server_msg("\n");
    wait_for_start();
}

void Robot::create(char *players)
{
    int now_players = 1, max_players = str_to_int(players);
    snd_server_msg(".create\n");
    rcv_server_msg();
    printf("Waiting now for %d\n", max_players - now_players);
    while (now_players < max_players)
    {
        rcv_server_msg();
        if (strstr(buffer, "JOIN"))
        {
            now_players++;
            printf("Waiting now for %d\n", max_players - now_players);
        }
    }
    printf("Everyone is present\n");
    snd_server_msg("start\n");
    rcv_server_msg();
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

char *Robot::parse_str(char **start, int &i)
{
    int j = 0;
    char *num = (char *)malloc(13);
    while (**start > '9' || **start < '0')
        (*start)++;
    while (**start != ' ' && **start != '\n')
    {
        num[j] = **start;
        j++;
        (*start)++;
    }
    num[j] = '\0';
    return num;
}

void Robot::parse(int **params, int params_num)
{
    int k, i = 0;
    char *start = strstr(buffer, nick);
    if (start == NULL)
        start = buffer;
    for (k = 0; k < params_num; ++k)
    {
        char *num = parse_str(&start, i);
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
        printf("Making %d prod...\n\n", raw);
        rcv_server_msg();
    }
}

void Robot::me()
{
    printf("INFORMATION about me (%s):\n", nick);
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
