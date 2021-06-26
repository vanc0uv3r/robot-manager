#include <stdio.h>
#include "Lex/Lex.h"
#include "Syntax/Syntax.h"
#include "Syntax/ErrorSyntax.h"
#include "Interpreter/Interpreter.h"
#include "Interpreter/ErrorInterpreter.h"
#include "RPN/RPNElem.h"
#include "helpers/helpers.h"

int lexeme_analyze(Lex &l)
{
    int c;
    while ((c = getchar()) != EOF && l.no_error())
        l.analyze(c);
    l.check_buffer();
    if (!l.no_error())
    {
        hdl_lex_error(&l);
        return 0;
    }
    return 1;
}

void print_rpn(RPNItem *list)
{
    while (list != NULL)
    {
        printf("%s", list->el->getPRNName());
        if (list->next != NULL)
            printf(" ");
        list = list->next;
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    Lex l;
    Syntax s;
    open_program(argv[1]);
    if (!lexeme_analyze(l))
        return 0;
    s.load_lexemes(l.get_lexemes());
    try
    {
        s.start_syntax();
        printf("OK\n");
    } catch (const ErrorSyntax &err_syn)
    {
        err_syn.error_msg();
    }

    print_rpn(s.get_rpn());

    Interpreter intr(s.get_rpn());

    try
    {
        intr.start();
    }
    catch (const ErrorInterpreter &err_syn)
    {
        err_syn.error_msg();
    }

    return 0;
}