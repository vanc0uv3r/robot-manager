#include "helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int str_to_int(char *s)
{
    int i, num = 0;
    if (strlen(s) > 12)
        return -1;
    for (i = 0; i < strlen(s); ++i)
    {
        if ((s[i] > '9' || s[i] < '0'))
            return -1;
        num = num * 10 + s[i] - '0';
    }
    return num;
}

void exit_with_perror(const char buffer[])
{
    perror(buffer);
    exit(1);
}

int check_params_num(int argc)
{
    return argc < 4 || argc > 5;
}

int check_params(int argc, char *argv[])
{
    if (check_params_num(argc))
    {
        printf("%s %s\n", invalid_params_num, usage);
        return quit;
    }
    else if (check_join(argc, argv))
        return join;
    else if (check_create(argc, argv))
        return create;
    return quit;
}

int check_join(int argc, char *argv[])
{
    return (argc == 5) && (strcmp(argv[3], "join") == 0)
    && (str_to_int(argv[4]) != -1);
}

int check_create(int argc, char *argv[])
{
    return (argc == 4) && (strcmp(argv[3], "create") == 0);
}