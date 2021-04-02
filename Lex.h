#ifndef ROBOT_MANAGER_LEX_H
#define ROBOT_MANAGER_LEX_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

enum machine_states
{
    num,
    key_word,
    str_const,
    variable,
    function,
    label,
    equal,
    arithmetic,
    none,
    pass,
    error,
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
    int quote;
    int lexeme_len;
    int line_number;
    int machine_state;
    char *buffer;

public:
    Lex();

    ~Lex(){ delete[] buffer;}

    int is_end_lexeme(char c);

    int is_delimiter(char c);

    int is_numeric(char c);

    int is_arithmetic(char c);

    int is_brackets(char c);

    int is_identifier(char c);

    int is_variable(char c);

    int is_function(char c);

    int is_label(char c);

    int is_equation(char c);

    int is_alpha(char c);

    int is_quote(char c);

    void num_handle(char c);

    void declaration_handle(char c);

    void keyword_handle(char c);

    void str_handle(char c);

    void arithmetic_handle(char c);

    void equation_handle(char c);

    void define_state(char c);

    void start_state(char c);

    void add_lexeme(char c);

    void add_buffer(char c);

    void analyze(char c);

    const char *define_lexeme_type();
};


#endif //ROBOT_MANAGER_LEX_H
