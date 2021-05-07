#include "RPNElem.h"
#include <stdio.h>

void RPNConstant::evaluate(RPNItem **stack, RPNItem **cur_cmd) const
{
    push(stack, clone());
    *cur_cmd = (*cur_cmd)->next;
}

void RPNElem::push(RPNItem **stack, RPNElem *el)
{

}

RPNElem *RPNElem::pop(RPNItem **stack)
{

}

RPNElem *RPNInt::clone() const
{
    return new RPNInt(value);
}

int RPNInt::get() const
{
    return value;
}

RPNElem *RPNString::clone() const
{
    return new RPNString(value);
}

char *RPNString::get() const {
    return value;
}

RPNString::RPNString(char *s)
{
    int str_len = strlen(s);
    value = new char[str_len + 1];
    strncpy(value, s, str_len);
    value[str_len] = '\0';
}

RPNLabel::RPNLabel(RPNElem *a)
{
    value = a;
}

RPNElem *RPNLabel::clone() const
{
    return new RPNLabel(value);
}

RPNElem *RPNLabel::get() const
{
    return value;
}

RPNElem *RPNVarAddr::clone() const {
    return new RPNVarAddr(value);
}

RPNElem *RPNVarAddr::get() const {
    return value;
}

RPNVarAddr::RPNVarAddr(RPNElem *a) {
    value = a;
}

void RPNFunction::evaluate(RPNItem **stack, RPNItem **cur_cmd) const
{
    RPNElem *res = evaluate_fun(stack);
    if (res)
        push(stack, res);
    *cur_cmd = (*cur_cmd)->next;
}

RPNElem *RPNFunPlus::evaluate_fun(RPNItem **stack) const
{
    int res;
    RPNInt *op1 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op1)
        throw 1;
    RPNInt *op2 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op2)
        throw 2;
    res = op1->get() + op2->get();
    return new RPNInt(res);
}

RPNElem *RPNFunMinus::evaluate_fun(RPNItem **stack) const {
    int res;
    RPNInt *op1 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op1)
        throw 1;
    RPNInt *op2 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op2)
        throw 2;
    res = op1->get() - op2->get();
    return new RPNInt(res);
}
