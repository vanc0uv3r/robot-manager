#ifndef ROBOT_MANAGER_ERRORSYNTAX_H
#define ROBOT_MANAGER_ERRORSYNTAX_H
#include <stdio.h>
#include <string.h>

class ErrorSyntax
{
    int line;
    char *msg;
    char *broken_lexeme;

public:
    ErrorSyntax(int lex_line, const char *str, char *lexeme)
    {
       int str_size = strlen(str);
       line = lex_line;
       broken_lexeme = NULL;
       msg = new char [str_size + 1];
       strncpy(msg, str, str_size);
       msg[str_size] = '\0';
       if (lexeme != NULL)
       {
           str_size = strlen(lexeme);
           broken_lexeme = new char [str_size + 1];
           strncpy(broken_lexeme, lexeme, str_size);
           broken_lexeme[str_size] = '\0';
       }
    }

    ErrorSyntax(const ErrorSyntax &err_syn)
    {
        int str_size = strlen(err_syn.msg);
        line = err_syn.line;
        msg = new char [str_size + 1];
        strncpy(msg, err_syn.msg, str_size);
        msg[str_size] = '\0';
        if (err_syn.broken_lexeme != NULL)
        {
            str_size = strlen(err_syn.broken_lexeme);
            broken_lexeme = new char[str_size + 1];
            strncpy(broken_lexeme, err_syn.broken_lexeme, str_size);
            broken_lexeme[str_size] = '\0';
        }
    }

    ~ErrorSyntax()
    {
        delete [] msg;
        if (broken_lexeme)
            delete [] broken_lexeme;
    }

    void error_msg() const
    {
        if (broken_lexeme)
            printf("%s on line %d. Found %s\n",  msg, line, broken_lexeme);
        else
            printf("%s on line %d\n", msg, line);
    }

};


#endif //ROBOT_MANAGER_ERRORSYNTAX_H
