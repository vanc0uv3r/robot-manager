#include "Robot.h"
#include <sys/socket.h>
#include <arpa/inet.h>

Robot::Robot(char *serv_ip, char *serv_port, char *nickname)
{
    ip = serv_ip;
    port = str_to_int(serv_port);
    nick = nickname;
    buffer_p = 0;
    buffer = (char *)malloc(init_buff_size * sizeof(*buffer));
    line = (char *)malloc(init_buff_size * sizeof(*buffer));
    connect_serv();
    enter_server();
}

void Robot::connect_serv()
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        exit_with_perror(sock_error);
    if (!inet_aton(ip, &(addr.sin_addr)))
        exit_with_perror(invalid_ip);
    if (connect(sockfd, (struct sockaddr *) &addr, sizeof(addr)))
        exit_with_perror(connect_error);
}

void Robot::enter_server()
{
    dprintf(sockfd, "%s\n", nick);
    rcv_server_msg();
    rcv_server_msg();
}

Robot::~Robot()
{
    free(buffer);
    free(line);
    close(sockfd);
}

void Robot::join(const char *room_num)
{
    dprintf(sockfd, ".join %s\n", room_num);
    wait_for_start();
}

void Robot::create(char *players)
{
    int now_players = 0, max_players = str_to_int(players);
    dprintf(sockfd, ".create\n");
    rcv_server_msg();
    while (now_players < max_players)
    {
        rcv_server_msg();
        if (strstr(line, "JOIN"))
        {
            now_players++;
            if (now_players < max_players)
                printf("Waiting now for %d\n", max_players - now_players);
        }
    }
    rcv_server_msg();
    printf("Everyone is present\n");
    dprintf(sockfd, "start\n");
    rcv_server_msg();
}

int Robot::check_end()
{
    for (int i = 0; i < strlen(buffer); i++)
    {
        if (buffer[i] == '\n')
            return i;
    }
    return 0;
}

void Robot::rcv_server_msg()
{
    int rc, cmd_pos, tmp_buff_size = init_buff_size;
    while ((cmd_pos = check_end()) == 0)
    {
        rc = read(sockfd, buffer + buffer_p * sizeof(*buffer), 32);
        if (rc == -1)
            perror("Read error: ");
        else if (rc == 0)
        {
            printf("%s\n", server_dis);
            exit(0);
        }
        buffer_p += rc;
        buffer[buffer_p] = '\0';
        //TODO buffer size check
    }
    strncpy(line, buffer, cmd_pos);
    line[cmd_pos] = '\0';
    buffer_shift(cmd_pos);
    buffer_p = strlen(buffer);
}

void Robot::buffer_shift(int cmd_pos)
{
    for (int i = 0; i < buffer_p; ++i)
    {
        if (i < buffer_p - cmd_pos + 1)
            buffer[i] = buffer[i + cmd_pos + 1];
        else
            buffer[i] = '\0';
    }
}

void Robot::parse()
{
    char r1, r2[7], r3[32];
    if (strstr(line, nick))
        sscanf(line, "%c%s%s%d%d%d%d%d", &r1, r2, r3, &raw, &prod, &money,
               &plants, &auto_plants);
    else if (strstr(line, "MARKET"))
        sscanf(line, "%c%s%d%d%d%d", &r1, r2, &m.raw, &m.min_price, &m.prod,
               &m.max_price);
}

void Robot::sell()
{
    if (prod > 0)
    {
        dprintf(sockfd, "sell %d %d\n", prod, m.max_price);
        printf("Selling %d prods now with price %d\n", prod, m.max_price);
        rcv_server_msg();
    }
}

void Robot::buy()
{
    dprintf(sockfd, "buy 2 %d\n", m.min_price);
    printf("Try to buy 2 raws which costs %d\n", m.min_price);
    rcv_server_msg();
}

void Robot::make_prod()
{
    if (raw > 0 && raw < 3)
    {
        dprintf(sockfd, "prod %d\n", raw);
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
    while (strstr(line, "ENDTURN") == NULL)
        rcv_server_msg();
}

void Robot::make_turn()
{
    dprintf(sockfd, "turn\n");
    rcv_server_msg();
}

void Robot::get_market()
{
    dprintf(sockfd, "market\n");
    rcv_server_msg();
    while (strstr(line, "-----"))
        rcv_server_msg();
    parse();
    rcv_server_msg();
}

void Robot::get_info()
{
    dprintf(sockfd, "info\n");
    rcv_server_msg();
    while (strstr(line, "-----"))
        rcv_server_msg();
    while (strstr(line, "INFO"))
    {
        parse();
        rcv_server_msg();
    }
    rcv_server_msg();
    rcv_server_msg();
}

void Robot::wait_for_start()
{
    rcv_server_msg();
    while (strstr(line, "START") == NULL)
        rcv_server_msg();
}

int Robot::define_winner()
{
    if (strstr(line, "WIN") != NULL)
    {
        if (strstr(line, "YOU_WIN"))
            printf("YOO! I win!\n");
        else
            printf("DAMN! I lose :((\n");
        return 1;
    }
    return 0;
}

int Robot::no_winner()
{
    dprintf(sockfd, "?\n");
    rcv_server_msg();
    while ((strstr(line, "game is over") == NULL) && (strstr(line,
                                                             "# Requested")) == NULL)
    {
        if (define_winner())
            return 0;
        rcv_server_msg();
    }
    return strstr(line, "game is over") == NULL;
}

void Robot::resize_buffer(int size)
{
    char *tmp = (char *)malloc(size * 2 * sizeof(*tmp));
    for (int i = 0; i < strlen(buffer); i++)
        tmp[i] = buffer[i];
    free(buffer);
    buffer = tmp;
}