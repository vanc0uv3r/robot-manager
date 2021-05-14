#ifndef ROBOT_MANAGER_INTERPRETER_H
#define ROBOT_MANAGER_INTERPRETER_H

#include <stdio.h>
#include <string.h>
#include "../RPN/RPNElem.h"

class Interpreter
{
    RPNItem *rpn_list;
    RPNItem *stack;
    var_list *vars;

public:

    Interpreter(RPNItem *list)
    {
        stack = NULL;
        rpn_list = list;
        vars = NULL;
    }

    void start()
    {
        while (rpn_list != NULL)
            rpn_list->el->evaluate(&stack, &rpn_list, &vars);
    }
};


#endif //ROBOT_MANAGER_INTERPRETER_H
