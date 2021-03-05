#include "helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int str_to_int(char *s)
{
    int i, num = 0, minus = 1;
    if (strlen(s) > 12)
        return -1;
    for (i = 0; i < strlen(s); ++i)
    {
        if (i == 0 && s[i] == '-')
            minus = -1;
        else
        {
            if ((s[i] > '9' || s[i] < '0'))
                return -1;
            num = num * 10 + s[i] - '0';
        }
    }
    return num * minus;
}

void exit_with_perror(char *buffer)
{
    perror(buffer);
    exit(1);
}

void check_params_num(int argc)
{
    if (argc != 3)
    {
        printf("Invalid number of params. Usage ./prog_name ip_addr port");
        exit(1);
    }
}
