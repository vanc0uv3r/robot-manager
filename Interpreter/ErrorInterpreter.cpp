#include "ErrorInterpreter.h"
#include <string.h>
#include <stdio.h>

ErrorInterpreter::ErrorInterpreter(const char *str)
{
    int str_size = strlen(str);
    msg = new char[str_size + 1];
    strncpy(msg, str, str_size);
    msg[str_size] = '\0';
}

ErrorInterpreter::ErrorInterpreter(const ErrorInterpreter &err_syn)
{
    int str_size = strlen(err_syn.msg);
    msg = new char[str_size + 1];
    strncpy(msg, err_syn.msg, str_size);
    msg[str_size] = '\0';
}

ErrorInterpreter::~ErrorInterpreter() {
    delete msg;
}

void ErrorInterpreter::error_msg() const
{
    printf("Error while executing program: %s\n", msg);
}
