#ifndef ROBOT_MANAGER_ERRORINTERPRETER_H
#define ROBOT_MANAGER_ERRORINTERPRETER_H


class ErrorInterpreter
{
    char *msg;

public:
    ErrorInterpreter(const char *str);

    ErrorInterpreter(const ErrorInterpreter &err_syn);

    ~ErrorInterpreter();

    void error_msg() const;
};

#endif //ROBOT_MANAGER_ERRORINTERPRETER_H
