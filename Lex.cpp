#include <string.h>
#include "Lex.h"

Lex::Lex()
{
    quote = 0;
    lexeme_len = 0;
    line_len = 1;
    line_number = 1;
    machine_state = none;
    last_machine_state = none;
    buffer = new char [128];
    lexeme_list = NULL;
}

int Lex::analyze(char c)
{
    current_c = c;
    if (machine_state == none || machine_state == pass)
        define_state();
    if (machine_state != error && machine_state != pass)
        start_state();
    if (current_c == '\n')
    {
        line_number++;
        line_len = 0;
        machine_state = none;
    }
    return machine_state;
}

int Lex::get_error_line()
{
    return line_number;
}

int Lex::get_error_position()
{
    return line_len;
}

void Lex::add_buffer()
{
    buffer[lexeme_len] = current_c;
    buffer[lexeme_len + 1] = '\0';
    lexeme_len++;
    line_len++;
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
        last_machine_state = machine_state;
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

list *Lex::get_lexemes()
{
    return lexeme_list;
}

void Lex::clean_lexemes()
{
    list *p;
    while (lexeme_list != NULL)
    {
        p = lexeme_list;
        lexeme_list = lexeme_list->next;
        delete[] p->l->name;
        delete p->l;
        delete p;
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
    else if (state == brackets)
        return "brackets";
    else if (state == delimiter)
        return "delimiter";
    else
        return "bug possibly";
}

void Lex::start_state()
{
    if (machine_state == num)
        num_handle();
    else if (machine_state == key_word)
        keyword_handle();
    else if (machine_state == str_const)
        str_handle();
    else if (machine_state == arithmetic || machine_state == brackets
    || machine_state == delimiter)
        simple_handle();
    else if (machine_state == equal)
        equation_handle();
    else if (machine_state == variable || machine_state == function
    || machine_state == label)
        declaration_handle();
    else if (machine_state != delimiter)
        machine_state = error;
}

void Lex::define_state()
{
    if (is_end_lexeme())
        machine_state = pass;
    else if (is_numeric())
        machine_state = num;
    else if (is_alpha())
        machine_state = key_word;
    else if (is_variable())
        machine_state = variable;
    else if (is_function())
        machine_state = function;
    else if (is_label())
        machine_state = label;
    else if (is_quote())
        machine_state = str_const;
    else if (is_equation())
        machine_state = equal;
    else if (is_arithmetic())
        machine_state = arithmetic;
    else if (is_delimiter())
        machine_state = delimiter;
    else if (is_brackets())
        machine_state = brackets;
    else
        machine_state = error;
    last_machine_state = machine_state;
}

void Lex::equation_handle()
{
    if (current_c == ':' || current_c == '=')
    {
        add_buffer();
        if (current_c == '=')
            add_lexeme(&lexeme_list);
    }
    else
        machine_state = error;
}

void Lex::str_handle()
{
    if (is_alpha() || is_numeric())
        add_buffer();
    else if (is_quote())
    {
        quote = !quote;
        if (!quote)
            add_lexeme(&lexeme_list);
    }
    else if (!is_quote())
        machine_state = error;
}

void Lex::keyword_handle()
{
    if (is_alpha())
        add_buffer();
    else if (is_delimiter() || is_end_lexeme() || is_arithmetic() || is_brackets())
    {
        add_lexeme(&lexeme_list);
        if (is_arithmetic() || is_delimiter() || is_brackets())
        {
            add_buffer();
            define_state();
            add_lexeme(&lexeme_list);
        }
    }
    else
        machine_state = error;
}

void Lex::declaration_handle()
{
    if (is_alpha() || is_numeric())
        add_buffer();
    else if (is_delimiter() || is_end_lexeme() || is_arithmetic() || is_brackets())
    {
        add_lexeme(&lexeme_list);
        if (is_arithmetic() || is_delimiter() || is_brackets())
        {
            add_buffer();
            define_state();
            add_lexeme(&lexeme_list);
        }
    }
    else if (!is_identifier())
        machine_state = error;
}

void Lex::num_handle()
{
    if (is_numeric())
        add_buffer();
    else if (is_delimiter() || is_end_lexeme() || is_arithmetic() || is_brackets())
    {
        add_lexeme(&lexeme_list);
        if (is_arithmetic() || is_delimiter() || is_brackets())
        {
            add_buffer();
            define_state();
            add_lexeme(&lexeme_list);
        }
    }
    else
        machine_state = error;
}

void Lex::simple_handle()
{
    add_buffer();
    add_lexeme(&lexeme_list);
}

int Lex::is_quote()
{
    return current_c == '\"';
}

int Lex::is_alpha()
{
    return (current_c >= 'a' && current_c <= 'z')
    || (current_c >= 'A' && current_c <= 'Z') || current_c == '_'
    || current_c == '-';
}

int Lex::is_equation()
{
    return current_c == ':';
}

int Lex::is_variable()
{
    return current_c == '$';
}

int Lex::is_function()
{
    return current_c == '?';
}

int Lex::is_label()
{
    return current_c == '@';
}

int Lex::is_identifier()
{
    return is_label() || is_function() || is_variable();
}


int Lex::is_brackets()
{
    return current_c == '(' || current_c == ')' || current_c == '['
    || current_c == ']' || current_c == '{' || current_c == '}';
}

int Lex::is_arithmetic()
{
    return current_c == '+' || current_c == '-' || current_c == '*'
    || current_c == '/' || current_c == '%' || current_c == '>'
    || current_c == '<' || current_c == '=' || current_c == '!'
    || current_c == '|' || current_c == '&';
}

int Lex::is_numeric()
{
    return current_c >= '0' && current_c <= '9';
}

int Lex::is_delimiter()
{
    return current_c == ';' || current_c == ',';
}

int Lex::is_end_lexeme()
{
    return current_c == ' ' || current_c == '\t' || current_c == '\n';
}