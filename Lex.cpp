#include <string.h>
#include "Lex.h"

Lex::Lex()
{
    quote = 0;
    lexeme_len = 0;
    line_number = 1;
    machine_state = none;
    buffer = new char [128];
    lexeme_list = NULL;
}

void Lex::analyze(char c)
{
    if (machine_state == none || machine_state == pass)
        define_state(c);
    start_state(c);
}

void Lex::add_buffer(char c)
{
    buffer[lexeme_len] = c;
    buffer[lexeme_len + 1] = '\0';
    lexeme_len++;
}

void Lex::add_lexeme(list **lexemes)
{
    if (*lexemes == NULL)
    {
        *lexemes = new list;
        (*lexemes)->l = new lexeme;
        (*lexemes)->l->name = new char[lexeme_len + 1];
        strncpy((*lexemes)->l->name, buffer, lexeme_len + 1);
        (*lexemes)->l->type = machine_state;
        (*lexemes)->l->line = line_number;
        (*lexemes)->next = NULL;
        machine_state = none;
        lexeme_len = 0;
    }
    else
        add_lexeme(&((*lexemes)->next));
}

void Lex::print_lexemes()
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

const char *Lex::define_lexeme_type(int state)
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
    else
        return "kek";
}

void Lex::start_state(char c)
{
    if (c == '\n')
        line_number++;
    if (machine_state == num)
        num_handle(c);
    else if (machine_state == key_word)
        keyword_handle(c);
    else if (machine_state == str_const)
        str_handle(c);
    else if (machine_state == arithmetic)
        arithmetic_handle(c);
    else if (machine_state == equal)
        equation_handle(c);
    else if (machine_state == variable || machine_state == function
    || machine_state == label)
        declaration_handle(c);
}

void Lex::define_state(char c)
{
    if (is_end_lexeme(c))
        machine_state = pass;
    else if (is_numeric(c))
        machine_state = num;
    else if (is_alpha(c))
        machine_state = key_word;
    else if (is_variable(c))
        machine_state = variable;
    else if (is_function(c))
        machine_state = function;
    else if (is_label(c))
        machine_state = label;
    else if (is_quote(c))
        machine_state = str_const;
    else if (is_equation(c))
        machine_state = equal;
    else if (is_arithmetic(c))
        machine_state = arithmetic;
}

void Lex::equation_handle(char c)
{
    if (c == ':' || c == '=')
    {
        add_buffer(c);
        if (c == '=')
            add_lexeme(&lexeme_list);
    }
    else
        machine_state = error;
}

void Lex::str_handle(char c)
{
    if (is_alpha(c))
        add_buffer(c);
    else if (is_quote(c))
    {
        quote = !quote;
        if (!quote)
            add_lexeme(&lexeme_list);
    }
    else if (!is_quote(c))
        machine_state = error;
}

void Lex::keyword_handle(char c)
{
    if (is_alpha(c))
        add_buffer(c);
    else if (is_delimiter(c))
    {
        add_lexeme(&lexeme_list);
        if (is_arithmetic(c))
        {
            add_buffer(c);
            add_lexeme(&lexeme_list);
        }
    }
    else
        machine_state = error;
}

void Lex::declaration_handle(char c)
{
    if (is_alpha(c) || is_numeric(c))
        add_buffer(c);
    else if (is_delimiter(c))
    {
        add_lexeme(&lexeme_list);
        if (is_arithmetic(c))
        {
            add_buffer(c);
            add_lexeme(&lexeme_list);
        }
    }
    else if (!is_identifier(c))
        machine_state = error;
}

void Lex::num_handle(char c)
{
    if (is_numeric(c))
        add_buffer(c);
    else if (is_delimiter(c))
    {
        add_lexeme(&lexeme_list);
        if (is_arithmetic(c))
        {
            add_buffer(c);
            add_lexeme(&lexeme_list);
        }
    }
    else
        machine_state = error;
}

void Lex::arithmetic_handle(char c)
{
    add_buffer(c);
    add_lexeme(&lexeme_list);
}

int Lex::is_quote(char c)
{
    return c == '\"';
}

int Lex::is_alpha(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int Lex::is_equation(char c)
{
    return c == ':';
}

int Lex::is_variable(char c)
{
    return c == '$';
}

int Lex::is_function(char c)
{
    return c == '?';
}

int Lex::is_label(char c)
{
    return c == '@';
}

int Lex::is_identifier(char c)
{
    return is_label(c) || is_function(c) || is_variable(c);
}


int Lex::is_brackets(char c)
{
    return c == '(' || c == ')' || c == '[' || c == ']';
}

int Lex::is_arithmetic(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%'
           || c == '>' || c == '<' || c == '=' || c == '!' || c == '|' || c == '&';
}

int Lex::is_numeric(char c)
{
    return c >= '0' && c <= '9';
}

int Lex::is_delimiter(char c)
{
    return c == '\n' ||c == ' ' || c == '\t' || c == ';' || c == ','
           || is_arithmetic(c) || is_brackets(c);
}

int Lex::is_end_lexeme(char c)
{
    return c == ' ' || c == '\t' || c == '\n';
}