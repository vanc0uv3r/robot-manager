#include "Robot.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include "Competitor.h"

Robot::Robot(char *serv_ip, char *serv_port, char *nickname)
{
    s.ip = serv_ip;
    s.port = str_to_int(serv_port);
    nick = nickname;
    buffer_p = 0;
    buffer_size = 128;
    competitors = NULL;
    buffer = new char[buffer_size];
    line = new char [buffer_size];
    connect_serv();
    enter_server();
}

void Robot::connect_serv()
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(s.port);
    if ((s.sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        exit_with_perror(sock_error);
    if (!inet_aton(s.ip, &(addr.sin_addr)))
        exit_with_perror(invalid_ip);
    if (connect(s.sockfd, (struct sockaddr *) &addr, sizeof(addr)))
        exit_with_perror(connect_error);
}

void Robot::enter_server()
{
    int buff_size;
    buff_size = snprintf(snd_buff, snd_buff_size, "%s\n", nick);
    snd_serv(buff_size);
    rcv_server_msg();
    rcv_server_msg();
}

Robot::~Robot()
{
    delete[] buffer;
    delete[] line;
    clear_competitors();
    close(s.sockfd);
}

void Robot::join(const char *room_num)
{  
    int buff_size;
    buff_size = snprintf(snd_buff, snd_buff_size, ".join %s\n", room_num);
    snd_serv(buff_size);
    wait_for_start();
}

void Robot::create(char *players)
{
    int now_players = 0, max_players = str_to_int(players);
    int buff_size;
    buff_size = snprintf(snd_buff, snd_buff_size, ".create\n");
    snd_serv(buff_size);
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
    buff_size = snprintf(snd_buff, snd_buff_size, "start\n");
    snd_serv(buff_size);
    rcv_server_msg();
}

int Robot::check_end()
{
    int len = strlen(buffer);
    for (int i = 0; i < len; i++)
    {
        if (buffer[i] == '\n')
            return i;
    }
    return 0;
}

void Robot::rcv_server_msg()
{
    int rc, cmd_pos;
    while ((cmd_pos = check_end()) == 0)
    {
        if (need_realloc())
        {
            buffer_size *= 2;
            resize_buffer();
            resize_line();
        }
        rc = read(s.sockfd, buffer + buffer_p, read_size - 1);
        check_read(rc);
        buffer_p += rc;
        buffer[buffer_p] = '\0';
    }
    strncpy(line, buffer, cmd_pos);
    line[cmd_pos] = '\0';
    buffer_shift(cmd_pos);
    buffer_p = strlen(buffer);
}

void Robot::check_read(int rc)
{
    if (rc == -1)
    {
        perror("Read error: ");
        exit(1);
    }
    else if (rc == 0)
    {
        printf("%s\n", server_dis);
        exit(0);
    }
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

void Robot::update_competitor()
{
    bool updated = 0;
    object_list *p = competitors;
    while (p != NULL && !updated)
    {
        if (strstr(line, p->c->get_nick()))
        {
            p->c->update_fields(line);
            updated = 1;
        }
        p = p->next;
    }
    if (!updated)
        add_competitor(&competitors);
}

void Robot::add_competitor(object_list **l)
{
    if (*l == NULL)
    {
        *l = new object_list;
        (*l)->c = new Competitor();
        (*l)->c->update_fields(line);
        (*l)->next = NULL;
    }
    else
        add_competitor(&((*l)->next));
}

void Robot::clear_competitors()
{
    object_list *p;
    while (competitors != NULL)
    {
        p = competitors;
        competitors = competitors->next;
        delete p->c;
        delete p; 
    }
}

void Robot::parse()
{
    char r1, r2[7], r3[32];
    if (strstr(line, nick))
        sscanf(line, "%c%s%s%d%d%d%d%d", &r1, r2, r3, &own.raw, &own.prod,
               &own.money, &own.plants, &own.auto_plants);
    else if (strstr(line, "MARKET"))
        sscanf(line, "%c%s%d%d%d%d", &r1, r2, &market.raw, 
                &market.min_price, &market.prod, &market.max_price);
    else if (strstr(line, "INFO"))
        update_competitor();
}

void Robot::snd_serv(int size)
{
    write(s.sockfd, snd_buff, size);
}

void Robot::sell()
{
    int buff_size;
    if (own.prod > 0)
    {
        buff_size = snprintf(snd_buff, snd_buff_size, "sell %d %d\n",
                own.prod, market.max_price);
        snd_serv(buff_size);
        printf("Selling %d prods now with price %d\n", own.prod,
               market.max_price);
        rcv_server_msg();
    }
}

void Robot::buy()
{
    int buff_size;
    buff_size = snprintf(snd_buff, snd_buff_size, "buy 2 %d\n", 
            market.min_price);
    snd_serv(buff_size);
    printf("Try to buy 2 raws which costs %d\n", market.min_price);
    rcv_server_msg();
}

void Robot::make_prod()
{
    int buff_size;
    if (own.raw > 0 && own.raw < 3)
    {
        buff_size = snprintf(snd_buff, snd_buff_size, "prod %d\n", own.raw);
        snd_serv(buff_size);
        printf("Making %d prod...\n\n", own.raw);
        rcv_server_msg();
    }
}

void Robot::me()
{
    printf("INFORMATION about me (%s):\n", nick);
    printf("My BALANCE is %d$\n", own.money);
    printf("I have %d RAWS\n", own.raw);
    printf("I have %d PROD\n\n", own.prod);
}

void Robot::get_competitors()
{
    object_list *p = competitors;
    while (p != NULL)
    {
        p->c->print_fields();
        p = p->next;
    }
}

void Robot::wait_other()
{
    while (strstr(line, "ENDTURN") == NULL)
        rcv_server_msg();
}

void Robot::make_turn()
{
    int buff_size;
    buff_size = snprintf(snd_buff, snd_buff_size, "turn\n");
    snd_serv(buff_size);
    rcv_server_msg();
}

void Robot::get_market()
{
    int buff_size;
    buff_size = snprintf(snd_buff, snd_buff_size, "market\n");
    snd_serv(buff_size);
    rcv_server_msg();
    while (strstr(line, "-----"))
        rcv_server_msg();
    parse();
    rcv_server_msg();
}

void Robot::get_info()
{
    int buff_size;
    buff_size = snprintf(snd_buff, snd_buff_size, "info\n");
    snd_serv(buff_size);
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
    int buff_size;
    buff_size = snprintf(snd_buff, snd_buff_size, "?\n");
    snd_serv(buff_size);
    rcv_server_msg();
    while (look_for_winner())
    {
        if (define_winner())
            return 0;
        rcv_server_msg();
    }
    return 1;
}

void Robot::resize_buffer()
{
    char *tmp = new char[buffer_size];
    strcpy(tmp, buffer);
    delete[] buffer;
    buffer = tmp;
}

void Robot::resize_line()
{
    char *tmp = new char[buffer_size];
    delete[] line;
    line = tmp;
}

int Robot::look_for_winner()
{
    return (strstr(line, "game is over") == NULL)
    && (strstr(line, "# Requested")) == NULL;
}
