#include "Interpreter.h"

void Interpreter::clean_vars(var_list *var_tbl)
{
    if (var_tbl != NULL)
    {
        clean_vars(var_tbl->next);
        delete var_tbl->name;
        delete var_tbl;
    }
}

void Interpreter::clean_rpn(RPNItem *curr_cmd)
{
    if (curr_cmd != NULL)
    {
        clean_rpn(curr_cmd->next);
        delete curr_cmd->el;
        delete curr_cmd;
    }
}

Interpreter::Interpreter(RPNItem *list)
{
    stack = NULL;
    rpn_list = list;
    begin_rpn = rpn_list;
    vars = NULL;
}

Interpreter::~Interpreter()
{
    clean_rpn(begin_rpn);
    clean_vars(vars);
}

void Interpreter::start()
{
    while (rpn_list != NULL)
        rpn_list->el->evaluate(&stack, &rpn_list, &vars);
}
