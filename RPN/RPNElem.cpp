#include <stdio.h>
#include "RPNElem.h"
#include "../Interpreter/ErrorInterpreter.h"

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
        int len = strlen(name);
        *var_tbl = new var_list;
        (*var_tbl)->value = value;
        (*var_tbl)->name = new char [len + 1];
        strncpy((*var_tbl)->name, name, len + 1);
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

RPNInt::RPNInt(int a)
{
    int size = 20;
    char *tmp = new char [size];
    value = a;
    snprintf(tmp, size, "%d", value);
    name = tmp;
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

char *RPNString::getPRNName() {
    return NULL;
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

const char *RPNLabel::getPRNName() {
    return "label";
}

RPNElem *RPNVarAddr::clone() const {
    return new RPNVarAddr(value);
}

char *RPNVarAddr::get() const {
    return value;
}

RPNVarAddr::RPNVarAddr(char *a) {
    int str_len = strlen(a);
    value = new char[str_len + 1];
    strncpy(value, a, str_len);
    value[str_len] = '\0';
}

const char *RPNVarAddr::getPRNName() {
    return get();
}

const char *RPNInt::getPRNName()
{
    return name;
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
        throw ErrorInterpreter("2 argument in + function must be integer");
    RPNInt *op2 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op2)
        throw ErrorInterpreter("1 argument in + function must be integer");
    res = op1->get() + op2->get();
    delete op1;
    delete op2;
    return new RPNInt(res);
}

const char *RPNFunPlus::getPRNName() {
    return "+";
}

RPNElem *RPNFunMinus::evaluate_fun(RPNItem **stack, var_list **vars) const {
    int res;
    RPNInt *op1 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op1)
        throw ErrorInterpreter("2 argument in - function must be integer");
    RPNInt *op2 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op2)
        throw ErrorInterpreter("1 argument in - function must be integer");
    res = op2->get() - op1->get();
    delete op1;
    delete op2;
    return new RPNInt(res);
}

const char *RPNFunMinus::getPRNName() {
    return "-";
}

RPNElem *RPNFunMultiply::evaluate_fun(RPNItem **stack, var_list **vars) const {
    int res;
    RPNInt *op1 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op1)
        throw ErrorInterpreter("2 argument in * function must be integer");
    RPNInt *op2 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op2)
        throw ErrorInterpreter("1 argument in * function must be integer");
    res = op1->get() * op2->get();
    delete op1;
    delete op2;
    return new RPNInt(res);
}

const char *RPNFunMultiply::getPRNName() {
    return "*";
}

RPNElem *RPNFunModulo::evaluate_fun(RPNItem **stack, var_list **vars) const {
    int res;
    RPNInt *op1 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op1)
        throw ErrorInterpreter("2 argument in % function must be integer");
    RPNInt *op2 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op2)
        throw ErrorInterpreter("1 argument in % function must be integer");
    res = op2->get() % op1->get();
    delete op1;
    delete op2;
    return new RPNInt(res);
}

const char *RPNFunModulo::getPRNName() {
    return "%";
}

void RPNOpGO::evaluate(RPNItem **stack, RPNItem **cur_cmd, var_list **vars) const
{
    RPNLabel *label = dynamic_cast<RPNLabel *>(pop(stack));
    if (!label)
        throw ErrorInterpreter("Label expected in OPGO");
    *cur_cmd = label->get();
    delete label;
}

const char *RPNOpGO::getPRNName() {
    return "!";
}

RPNElem *RPNFunDivision::evaluate_fun(RPNItem **stack, var_list **vars) const {
    int res;
    RPNInt *op1 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op1)
        throw ErrorInterpreter("2 argument in / function must be integer");
    RPNInt *op2 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op2)
        throw ErrorInterpreter("1 argument in / function must be integer");
    res = op2->get() / op1->get();
    delete op1;
    delete op2;
    return new RPNInt(res);
}

const char *RPNFunDivision::getPRNName() {
    return "/";
}

void RPNOpGOFalse::evaluate(RPNItem **stack, RPNItem **cur_cmd, var_list **vars)
const
{
    RPNLabel *label = dynamic_cast<RPNLabel *>(pop(stack));
    if (!label)
        throw ErrorInterpreter("Label expected in OPGOFalse");
    RPNInt *cond = dynamic_cast<RPNInt *>(pop(stack));
    if (!cond)
        throw ErrorInterpreter("Condition expected");
    if (!cond->get())
    {
        *cur_cmd = label->get();
        delete label;
    }
    else
        *cur_cmd = (*cur_cmd)->next;
}

const char *RPNOpGOFalse::getPRNName() {
    return "!F";
}

RPNElem *RPNVar::evaluate_fun(RPNItem **stack, var_list **vars) const
{
    var_list *var;
    RPNVarAddr *op = dynamic_cast<RPNVarAddr *>(pop(stack));
    if (!op)
        throw ErrorInterpreter("VarAddr expected");
    var = find_var(*vars, op->get());
    if (!var)
        throw ErrorInterpreter("There is no such name of variable");
    delete op;
    return new RPNInt(var->value);
}

const char *RPNVar::getPRNName() {
    return "&";
}

RPNElem *RPNAssign::evaluate_fun(RPNItem **stack, var_list **vars) const
{
    RPNInt *op1 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op1)
        throw ErrorInterpreter("Argument in assigment must be integer");
    RPNVarAddr *op2 = dynamic_cast<RPNVarAddr *>(pop(stack));
    if (!op2)
        throw ErrorInterpreter("Argument in assigment must be integer");
    if (find_var(*vars, op2->get()))
        replace(*vars, op2->get(), op1->get());
    else
        add_var(vars, op1->get(), op2->get());
    delete op1;
    delete op2;
    return NULL;
}

const char *RPNAssign::getPRNName() {
    return ":=";
}

RPNElem *RPNNot::evaluate_fun(RPNItem **stack, var_list **vars) const {
    int res;
    RPNInt *op1 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op1)
        throw ErrorInterpreter("argument in not function must be integer");
    res = !(op1->get());
    delete op1;
    return new RPNInt(res);
}

const char *RPNNot::getPRNName() {
    return "!";
}

RPNElem *RPNMinus::evaluate_fun(RPNItem **stack, var_list **vars) const {
    int res;
    RPNInt *op1 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op1)
        throw ErrorInterpreter("argument in unary minus function must be "
                               "integer");
    res = -(op1->get());
    delete op1;
    return new RPNInt(res);
}

const char *RPNMinus::getPRNName() {
    return "-";
}

RPNElem *RPNMore::evaluate_fun(RPNItem **stack, var_list **vars) const {
    int res;
    RPNInt *op1 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op1)
        throw ErrorInterpreter("2 argument in > function must be integer");
    RPNInt *op2 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op2)
        throw ErrorInterpreter("1 argument in > function must be integer");
    res = op2->get() > op1->get();
    delete op1;
    delete op2;
    return new RPNInt(res);
}

const char *RPNMore::getPRNName() {
    return ">";
}

RPNElem *RPNLess::evaluate_fun(RPNItem **stack, var_list **vars) const {
    int res;
    RPNInt *op1 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op1)
        throw ErrorInterpreter("2 argument in < function must be integer");
    RPNInt *op2 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op2)
        throw ErrorInterpreter("1 argument in < function must be integer");
    res = op2->get() < op1->get();
    delete op1;
    delete op2;
    return new RPNInt(res);
}

const char *RPNLess::getPRNName() {
    return "<";
}

RPNElem *RPNEquals::evaluate_fun(RPNItem **stack, var_list **vars) const {
    int res;
    RPNInt *op1 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op1)
        throw ErrorInterpreter("2 argument in = function must be integer");
    RPNInt *op2 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op2)
        throw ErrorInterpreter("1 argument in = function must be integer");
    res = (op2->get() == op1->get());
    delete op1;
    delete op2;
    return new RPNInt(res);
}

const char *RPNEquals::getPRNName() {
    return "=";
}

RPNElem *RPNAnd::evaluate_fun(RPNItem **stack, var_list **vars) const {
    int res;
    RPNInt *op1 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op1)
        throw ErrorInterpreter("2 argument in and function must be int");
    RPNInt *op2 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op2)
        throw ErrorInterpreter("1 argument in and function must be int");
    res = op2->get() && op1->get();
    delete op1;
    delete op2;
    return new RPNInt(res);
}

const char *RPNAnd::getPRNName() {
    return "and";
}

RPNElem *RPNOr::evaluate_fun(RPNItem **stack, var_list **vars) const {
    int res;
    RPNInt *op1 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op1)
        throw ErrorInterpreter("2 argument in or function must be integer");
    RPNInt *op2 = dynamic_cast<RPNInt *>(pop(stack));
    if (!op2)
        throw ErrorInterpreter("1 argument in or function must be integer");
    res = op2->get() || op1->get();
    delete op1;
    delete op2;
    return new RPNInt(res);
}

const char *RPNOr::getPRNName() {
    return "or";
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

const char *RPNSell::getPRNName() {
    return "sell";
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
    {
        printf("%d", op1->get());
        delete op1;
    }
    else if ((op2 = dynamic_cast<RPNString *>(op)))
    {
        printf("%s", op2->get());
        delete op2;
    }
    return NULL;
}

const char *RPNPrint::getPRNName() {
    return "print";
}

RPNElem *RPNMyId::evaluate_fun(RPNItem **stack, var_list **vars) const {
    return NULL;
}

const char *RPNMyId::getPRNName() {
    return "my_id";
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

const char *RPNNOP::getPRNName() {
    return "nop";
}
