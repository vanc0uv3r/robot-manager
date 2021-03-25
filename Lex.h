#ifndef ROBOT_MANAGER_LEX_H
#define ROBOT_MANAGER_LEX_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

enum lexeme_types
{
    separator,
    num_const,
    str_const,
    variable,
    label,
    function,
    key_word
};

struct lexeme
{
    int line;
    int type;
    char *name;
};

struct list
{
    lexeme *l;
    list *next;
};

class Lex
{
    int fd;
    int c;
    char *buffer;
    lexeme l;
    list lexeme_list;

public:
    Lex(const char *filename);

    ~Lex(){ delete[] buffer;}

    int define_lexeme_type();

    int next_lexeme();
};


#endif //ROBOT_MANAGER_LEX_H
