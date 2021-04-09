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
    delimiter,
    brackets
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
    int line_len;
    int lexeme_len;
    int line_number;
    int machine_state;
    char *buffer;
    char current_c;
    list *lexeme_list;

    int is_end_lexeme();

    int is_delimiter();

    int is_numeric();

    int is_arithmetic();

    int is_brackets();

    int is_identifier();

    int is_variable();

    int is_function();

    int is_label();

    int is_equation();

    int is_alpha();

    int is_quote();

    void num_handle();

    void declaration_handle();

    void keyword_handle();

    void str_handle();

    void simple_handle();

    void equation_handle();

    void define_state();

    void start_state();

    void add_lexeme(list **lexemes);

    void print_lexemes();

    void add_buffer();

    void clean_lexemes();

public:
    Lex();

    ~Lex(){ delete[] buffer; clean_lexemes();}

    list *get_lexemes();

    int analyze(char c);

    int get_error_line();

    int get_error_position();

    const char *define_lexeme_type(int state);
};


#endif //ROBOT_MANAGER_LEX_H
