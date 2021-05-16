#include "helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Lex/Lex.h"

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

void open_program(const char *filename)
{
    int fd;
    fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        perror("file");
        exit(1);
    }
    dup2(fd, 0);
    close(fd);
}

const char *define_lexeme_type(int state)
{
    if (state == num)
        return "number";
    else if (state == key_word)
        return "key word";
    else if (state == label)
        return "label";
    else if (state == function)
        return "function";
    else if (state == variable)
        return "variable";
    else if (state == str_const)
        return "string";
    else if (state == equal)
        return "equation";
    else if (state == arithmetic)
        return "arithmetic";
    else if (state == brackets)
        return "brackets";
    else if (state == delimiter)
        return "delimiter";
    else
        return "bug possibly";
}

void print_lexemes(list *lexeme_list)
{
    list *p = lexeme_list;
    while (p != NULL)
    {
        printf("lexeme: '%s', ", p->l->name);
        printf("lexeme type: '%s', ", define_lexeme_type(p->l->type));
        printf("line_number: %d\n", p->l->line);
        p = p->next;
    }
}

void hdl_lex_error(Lex *l)
{
    print_lexemes(l->get_lexemes());
    printf("%s error in line %d, position - %d in %c symbol\n",
           define_lexeme_type(l->get_last_machine_state()),
           l->get_error_line(), l->get_error_position(),
           l->get_last_char());
}