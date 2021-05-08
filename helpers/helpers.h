#ifndef ROBOT_MANAGER_HELPERS_H
#define ROBOT_MANAGER_HELPERS_H

#include "../Lex/Lex.h"

#define snd_buff_size 100

enum next_action {
    join,
    create,
    quit
};

const int read_size = 32;
const int str_int_size = 12;
const char sock_error[] = "Socket problem";
const char invalid_params_num[] = "Invalid number of params.";
const char invalid_params[] = "Invalid params.";
const char usage[] = "Usage ./robot <ip_addr> <port> <bot_name> join <room_num>"
                     " | create <max_players>";
const char invalid_ip[] = "Invalid ip addr";
const char server_dis[] = "Server decided to close connection. Exiting...";
const char connect_error[] = "Error while connect to server";

int str_to_int(char *s);

void exit_with_perror(const char buffer[]);

int check_params_num(int argc);

int check_params(int argc, char *argv[]);

int check_join(char *argv[]);

int check_create(char *argv[]);

int check_nickname(char *nickname);

char *reverse_str(char *str);

void hdl_lex_error(Lex *l);

void print_lexemes(list *lexeme_list);

const char *define_lexeme_type(int state);

void open_program(const char *filename);

#endif //ROBOT_MANAGER_HELPERS_H
