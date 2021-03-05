#ifndef ROBOT_MANAGER_HELPERS_H
#define ROBOT_MANAGER_HELPERS_H

const char sock_error[] = "Socket problem";
const char invalid_params[] = "Invalid number of params. Usage ./prog_name "
                              "ip_addr port\n";
const char invalid_ip[] = "Invalid ip addr";
const char connect_error[] = "Error while connect to server";

int str_to_int(char *s);

void exit_with_perror(const char buffer[]);

void check_params_num(int argc);


#endif //ROBOT_MANAGER_HELPERS_H
