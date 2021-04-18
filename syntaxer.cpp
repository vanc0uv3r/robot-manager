#include <stdio.h>
#include "helpers.h"
#include "Lex.h"

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

int main(int argc, char *argv[])
{
    int c;
    int current_state = none;
    Lex l;
    list *lexeme_list = NULL;
    open_program(argv[1]);
    while ((c = getchar()) != EOF && current_state != error)
        current_state = l.analyze(c);
    if (current_state == error)
    {
        printf("Error in line %d, position - %d\n", l.get_error_line(),
               l.get_error_position());
    }
    else
    {
        lexeme_list = l.get_lexemes();
        print_lexemes(lexeme_list);
    }

    return 0;
}
