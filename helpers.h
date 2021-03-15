#ifndef ROBOT_MANAGER_HELPERS_H
#define ROBOT_MANAGER_HELPERS_H

#define BUFFER_SIZE 1024
#define STR_INT_SIZE 12

enum next_action {
    join,
    create,
    quit
};

const char sock_error[] = "Socket problem";
const char invalid_params_num[] = "Invalid number of params.";
const char invalid_params[] = "Invalid params.";
const char usage[] = "Usage ./prog_name <ip_addr> <port> join <room_num> | "
                     "create";
const char invalid_ip[] = "Invalid ip addr";
const char server_dis[] = "Server decided to close connection. Exiting...";
const char connect_error[] = "Error while connect to server";

int str_to_int(char *s);

void exit_with_perror(const char buffer[]);

int check_params_num(int argc);

int check_params(int argc, char *argv[]);

int check_join(int argc, char *argv[]);

int check_create(int argc, char *argv[]);

char *reverse_str(char *str);

char *int_to_str(int num);

#endif //ROBOT_MANAGER_HELPERS_H
