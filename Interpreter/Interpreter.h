#ifndef ROBOT_MANAGER_INTERPRETER_H
#define ROBOT_MANAGER_INTERPRETER_H

#include <stdio.h>
#include <string.h>
#include "../RPN/RPNElem.h"

class Interpreter
{
    RPNItem *rpn_list;
    RPNItem *begin_rpn;
    RPNItem *stack;
    var_list *vars;

    void clean_rpn(RPNItem *curr_cmd);

    void clean_vars(var_list *var_tbl);

public:

    Interpreter(RPNItem *list);

    ~Interpreter();

    void start();
};


#endif //ROBOT_MANAGER_INTERPRETER_H
