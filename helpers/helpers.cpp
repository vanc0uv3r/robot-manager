#include "helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int str_to_int(char *s)
{
    int i, num = 0, len = strlen(s);
    if (strlen(s) > str_int_size)
        return -1;
    for (i = 0; i < len; ++i)
    {
        if ((s[i] > '9' || s[i] < '0'))
            return -1;
        num = num * 10 + s[i] - '0';
    }
    return num;
}

char *int_to_str(int num)
{
    char *s;
    int i = 0;
    if (num < 0)
        return NULL;
    s = new char[str_int_size + 1];
    while (num != 0)
    {
        s[i] = '0' + (num % 10);
        num /= 10;
        i++;
    }
    s[i] = '\0';
    return reverse_str(s);
}

char *reverse_str(char *str)
{
    char *p1, *p2;

    if (str == NULL || *str == '\0')
        return str;
    p1 = str;
    p2 = str + strlen(str) - 1;
    while (p2 > p1)
    {
        *p1 ^= *p2;
        *p2 ^= *p1;
        *p1 ^= *p2;
        ++p1, --p2;
    }
    return str;
}

void exit_with_perror(const char buffer[])
{
    perror(buffer);
    exit(1);
}

int check_params_num(int argc)
{
    return argc != 6;
}

int check_params(int argc, char *argv[])
{
    if (check_params_num(argc))
    {
        printf("%s %s\n", invalid_params_num, usage);
        return quit;
    }
    else if (check_nickname(argv[3]))
    {
        printf("%s %s\n", invalid_params, usage);
        return quit;
    }
    else if (check_join(argv))
        return join;
    else if (check_create(argv))
        return create;
    return quit;
}

int check_join(char *argv[])
{
    return (strcmp(argv[4], "join") == 0)
    && (str_to_int(argv[5]) != -1);
}

int check_create(char *argv[])
{
    return (strcmp(argv[4], "create") == 0)
    && (str_to_int(argv[5]) != -1);
}

int check_nickname(char *nickname)
{
    return strlen(nickname) > 16;
}
