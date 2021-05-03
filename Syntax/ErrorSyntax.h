#ifndef ROBOT_MANAGER_ERRORSYNTAX_H
#define ROBOT_MANAGER_ERRORSYNTAX_H
#include <stdio.h>
#include <string.h>

class ErrorSyntax
{
    int line;
    char *msg;
    char *broken_lexeme;

    void copy_str(char *lexeme, int str_size);

public:
    ErrorSyntax(int lex_line, const char *str, char *lexeme);

    ErrorSyntax(const ErrorSyntax &err_syn);

    ~ErrorSyntax();

    void error_msg() const;

};


#endif //ROBOT_MANAGER_ERRORSYNTAX_H
