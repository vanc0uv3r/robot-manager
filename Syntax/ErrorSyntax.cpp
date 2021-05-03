#include "ErrorSyntax.h"


ErrorSyntax::ErrorSyntax(int lex_line, const char *str, char *lexeme)
{
    int str_size = strlen(str);
    line = lex_line;
    broken_lexeme = NULL;
    msg = new char[str_size + 1];
    strncpy(msg, str, str_size);
    msg[str_size] = '\0';
    copy_str(lexeme, str_size);
}

ErrorSyntax::ErrorSyntax(const ErrorSyntax &err_syn)
{
    int str_size = strlen(err_syn.msg);
    line = err_syn.line;
    msg = new char[str_size + 1];
    strncpy(msg, err_syn.msg, str_size);
    msg[str_size] = '\0';
    copy_str(err_syn.broken_lexeme, str_size);
}

ErrorSyntax::~ErrorSyntax()
{
    delete [] msg;
    if (broken_lexeme)
        delete [] broken_lexeme;
}

void ErrorSyntax::copy_str(char *lexeme, int str_size)
{
    if (lexeme != NULL)
    {
        str_size = strlen(lexeme);
        broken_lexeme = new char[str_size + 1];
        strncpy(broken_lexeme, lexeme, str_size);
        broken_lexeme[str_size] = '\0';
    }
}

void ErrorSyntax::error_msg() const
{
    if (broken_lexeme)
        printf("%s on line %d. Found %s\n",  msg, line, broken_lexeme);
    else
        printf("%s on line %d\n", msg, line);
}
