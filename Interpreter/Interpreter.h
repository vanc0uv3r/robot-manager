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

    void clean_up(RPNItem *curr_cmd)
    {
        if (curr_cmd != NULL)
        {
            clean_up(curr_cmd->next);
            delete curr_cmd->el;
            delete curr_cmd;
        }
    }

public:

    Interpreter(RPNItem *list)
    {
        stack = NULL;
        rpn_list = list;
        begin_rpn = rpn_list;
        vars = NULL;
    }

    ~Interpreter() {clean_up(begin_rpn);}

    void start()
    {
        while (rpn_list != NULL)
            rpn_list->el->evaluate(&stack, &rpn_list, &vars);
    }
};


#endif //ROBOT_MANAGER_INTERPRETER_H
