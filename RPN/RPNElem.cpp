#include <stdio.h>
#include "RPNElem.h"


void RPNConstant::evaluate(RPNItem **stack, RPNItem **cur_cmd, var_list **vars) 
const
{
    push(stack, clone());
    *cur_cmd = (*cur_cmd)->next;
}

void RPNElem::push(RPNItem **stack, RPNElem *el)
{
    RPNItem *tmp = *stack;
    (*stack) = new RPNItem;
    (*stack)->el = el;
    (*stack)->next = tmp;
}

RPNElem *RPNElem::pop(RPNItem **stack)
{
    RPNItem *tmp_i = (*stack);
    RPNElem *tmp_e = (*stack)->el;
    (*stack) = (*stack)->next;
    delete tmp_i;
    return tmp_e;
}

void RPNElem::add_var(var_list **var_tbl, int value, char *name)
{
    if (*var_tbl == NULL)
    {
        *var_tbl = new var_list;
        (*var_tbl)->value = value;
        (*var_tbl)->name = name;
        (*var_tbl)->next = NULL;
    }
    else
        add_var(&((*var_tbl)->next), value, name);
}

var_list *RPNElem::find_var(var_list *var_tbl, char *target) {
    var_list *tmp = var_tbl;
    while (tmp != NULL && strcmp(tmp->name, target))
        tmp = tmp->next;
    return tmp;
}

void RPNElem::replace(var_list *var_tbl, char *target, int value)
{
    var_list *tmp = var_tbl;
    while (tmp != NULL)
    {
        if (!strcmp(tmp->name, target))
            tmp->value = value;
        tmp = tmp->next;
    }
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

RPNLabel::RPNLabel(RPNItem *a)
{
    value = a;
}

RPNElem *RPNLabel::clone() const
{
    return new RPNLabel(value);
}

RPNItem *RPNLabel::get() const
{
    return value;
}

RPNElem *RPNVarAddr::clone() const {
    return new RPNVarAddr(value);
}

char *RPNVarAddr::get() const {
    return value;
}

RPNVarAddr::RPNVarAddr(char *a) {
    value = a;
}

void RPNFunction::evaluate(RPNItem **stack, RPNItem **cur_cmd, var_list **vars) 
const
{
    RPNElem *res = evaluate_fun(stack, vars);
    if (res)
        push(stack, res);
    *cur_cmd = (*cur_cmd)->next;
}

RPNElem *RPNFunPlus::evaluate_fun(RPNItem **stack, var_list **vars) const
{
    int res;
    RPNInt *op1 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op1)
        throw 1;
    RPNInt *op2 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op2)
        throw 2;
    res = op1->get() + op2->get();
    delete op1;
    delete op2;
    return new RPNInt(res);
}

RPNElem *RPNFunMinus::evaluate_fun(RPNItem **stack, var_list **vars) const {
    int res;
    RPNInt *op1 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op1)
        throw 1;
    RPNInt *op2 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op2)
        throw 2;
    res = op2->get() - op1->get();
    return new RPNInt(res);
}

RPNElem *RPNFunMultiply::evaluate_fun(RPNItem **stack, var_list **vars) const {
    int res;
    RPNInt *op1 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op1)
        throw 1;
    RPNInt *op2 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op2)
        throw 2;
    res = op1->get() * op2->get();
    return new RPNInt(res);
}

RPNElem *RPNFunModulo::evaluate_fun(RPNItem **stack, var_list **vars) const {
    int res;
    RPNInt *op1 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op1)
        throw 1;
    RPNInt *op2 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op2)
        throw 2;
    res = op2->get() % op1->get();
    return new RPNInt(res);
}

void RPNOpGO::evaluate(RPNItem **stack, RPNItem **cur_cmd, var_list **vars) const
{
    RPNLabel *label = dynamic_cast<RPNLabel *>(pop(stack));
    if (!label)
        throw 1;
    *cur_cmd = label->get();
    delete label;
}

RPNElem *RPNFunDivision::evaluate_fun(RPNItem **stack, var_list **vars) const {
    int res;
    RPNInt *op1 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op1)
        throw 1;
    RPNInt *op2 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op2)
        throw 2;
    res = op2->get() / op1->get();
    return new RPNInt(res);
}

void RPNOpGOFalse::evaluate(RPNItem **stack, RPNItem **cur_cmd, var_list **vars)
const
{
    RPNLabel *label = dynamic_cast<RPNLabel *>(pop(stack));
    if (!label)
        throw 1;
    RPNInt *cond = dynamic_cast<RPNInt *>(pop(stack));
    if (!cond)
        throw 2;
    if (!cond->get())
    {
        *cur_cmd = label->get();
        delete label;
    }
    else
        *cur_cmd = (*cur_cmd)->next;
}

RPNElem *RPNVar::evaluate_fun(RPNItem **stack, var_list **vars) const
{
    var_list *var;
    RPNVarAddr *op = dynamic_cast<RPNVarAddr *>(pop(stack));
    if (!op)
        throw 2;
    var = find_var(*vars, op->get());
    if (!var)
        throw 1;
    delete op;
    return new RPNInt(var->value);
}

RPNElem *RPNAssign::evaluate_fun(RPNItem **stack, var_list **vars) const
{
    RPNInt *op1 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op1)
        throw 1;
    RPNVarAddr *op2 = dynamic_cast<RPNVarAddr *>(pop(stack));
    if (!op2)
        throw 1;
    if (find_var(*vars, op2->get()))
        replace(*vars, op2->get(), op1->get());
    else
        add_var(vars, op1->get(), op2->get());
    return NULL;
}

RPNElem *RPNNot::evaluate_fun(RPNItem **stack, var_list **vars) const {
    RPNInt *op1 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op1)
        throw 1;
    return new RPNInt(!(op1->get()));
}

RPNElem *RPNMinus::evaluate_fun(RPNItem **stack, var_list **vars) const {
    RPNInt *op1 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op1)
        throw 1;
    return new RPNInt(-(op1->get()));
}

RPNElem *RPNMore::evaluate_fun(RPNItem **stack, var_list **vars) const {
    int res;
    RPNInt *op1 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op1)
        throw 1;
    RPNInt *op2 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op2)
        throw 2;
    res = op2->get() > op1->get();
    return new RPNInt(res);
}

RPNElem *RPNLess::evaluate_fun(RPNItem **stack, var_list **vars) const {
    int res;
    RPNInt *op1 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op1)
        throw 1;
    RPNInt *op2 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op2)
        throw 2;
    res = op2->get() < op1->get();
    return new RPNInt(res);
}

RPNElem *RPNEquals::evaluate_fun(RPNItem **stack, var_list **vars) const {
    int res;
    RPNInt *op1 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op1)
        throw 1;
    RPNInt *op2 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op2)
        throw 2;
    res = (op2->get() == op1->get());
    return new RPNInt(res);
}

RPNElem *RPNAnd::evaluate_fun(RPNItem **stack, var_list **vars) const {
    int res;
    RPNInt *op1 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op1)
        throw 1;
    RPNInt *op2 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op2)
        throw 2;
    res = op2->get() && op1->get();
    return new RPNInt(res);
}

RPNElem *RPNOr::evaluate_fun(RPNItem **stack, var_list **vars) const {
    int res;
    RPNInt *op1 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op1)
        throw 1;
    RPNInt *op2 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op2)
        throw 2;
    res = op2->get() || op1->get();
    return new RPNInt(res);
}

RPNElem *RPNBuy::evaluate_fun(RPNItem **stack, var_list **vars) const{
    return NULL;
}

RPNElem *RPNEndTurn::evaluate_fun(RPNItem **stack, var_list **vars) const {
    return NULL;
}

RPNElem *RPNProd::evaluate_fun(RPNItem **stack, var_list **vars) const {
    return NULL;
}

RPNElem *RPNSell::evaluate_fun(RPNItem **stack, var_list **vars) const {
    return NULL;
}

RPNElem *RPNBuild::evaluate_fun(RPNItem **stack, var_list **vars) const {
    return NULL;
}

RPNElem *RPNPrint::evaluate_fun(RPNItem **stack, var_list **vars) const
{
    RPNElem *op = pop(stack);
    RPNInt *op1;
    RPNString *op2;
    if ((op1 = dynamic_cast<RPNInt *>(op)))
        printf("%d", op1->get());
    else if ((op2 = dynamic_cast<RPNString *>(op)))
        printf("%s", op2->get());
    return NULL;
}

RPNElem *RPNMyId::evaluate_fun(RPNItem **stack, var_list **vars) const {
    return NULL;
}

RPNElem *RPNTurn::evaluate_fun(RPNItem **stack, var_list **vars) const {
    return NULL;
}

RPNElem *RPNMoney::evaluate_fun(RPNItem **stack, var_list **vars) const {
    return NULL;
}

RPNElem *RPNRaw::evaluate_fun(RPNItem **stack, var_list **vars) const {
    return NULL;
}
