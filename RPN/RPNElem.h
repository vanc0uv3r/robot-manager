#ifndef ROBOT_MANAGER_RPNELEM_H
#define ROBOT_MANAGER_RPNELEM_H
#include <string.h>

class Interpreter;

struct var_list
{
    char *name;
    int value;
    var_list *next;
};

class RPNElem;

struct RPNItem
{
    RPNElem *el;
    RPNItem *next;
};

class RPNElem
{
public:
    virtual void evaluate(RPNItem **stack, RPNItem **cur_cmd, var_list **vars)
    const = 0;
    virtual ~RPNElem() {}
protected:
    static void push(RPNItem **stack, RPNElem *el);

    static RPNElem *pop(RPNItem **stack);

    static void add_var(var_list **var_tbl, int value, char *name);

    static var_list *find_var(var_list *var_tbl, char *target);

    static void replace(var_list *var_tbl, char *target, int value);
};

class RPNConstant : public RPNElem
{
public:
    virtual RPNElem *clone() const = 0;
    void evaluate(RPNItem **stack, RPNItem **cur_cmd, var_list **vars) const;
};

class RPNFunction : public RPNElem
{
public:
    void evaluate(RPNItem **stack, RPNItem **cur_cmd, var_list **vars) const;
    virtual RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const = 0;
};

class RPNInt : public RPNConstant
{
    int value;
public:
    RPNInt(int a) {value = a;}
    RPNElem *clone() const;
    int get() const;
    ~RPNInt() {}
};

class RPNString : public RPNConstant
{
    char *value;
public:
    RPNString(char *s);
    RPNElem *clone() const;
    char *get() const;
    ~RPNString() {delete value;}
};

class RPNLabel : public RPNConstant
{
    RPNItem *value;
public:
    RPNLabel(RPNItem *a = NULL);
    RPNElem *clone() const;
    RPNItem *get() const;
    ~RPNLabel() {delete value;}
    void set(RPNItem *addr) {value = addr;}
};

class RPNVarAddr : public RPNConstant
{
    char *value;
public:
    RPNVarAddr(char *a);
    RPNElem *clone() const;
    char *get() const;
    ~RPNVarAddr() {delete value;}
};

class RPNFunPlus : public RPNFunction
{
public:
    ~RPNFunPlus() {}
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNFunMinus : public RPNFunction
{
public:
    ~RPNFunMinus() {}
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNFunMultiply : public RPNFunction
{
public:
    ~RPNFunMultiply() {}
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNFunModulo : public RPNFunction
{
public:
    ~RPNFunModulo() {}
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNFunDivision : public RPNFunction
{
public:
    ~RPNFunDivision() {}
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNOpGO : public RPNElem
{
public:
    ~RPNOpGO() {}
    void evaluate(RPNItem **stack, RPNItem **cur_cmd, var_list **vars) const;
};

class RPNOpGOFalse : public RPNElem
{
public:
    ~RPNOpGOFalse() {}
    void evaluate(RPNItem **stack, RPNItem **cur_cmd, var_list **vars) const;
};

class RPNVar : public RPNFunction
{
public:
    RPNVar() {}
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNAssign : public RPNFunction
{
public:
    RPNAssign() {}
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNNot : public RPNFunction
{
public:
    RPNNot() {}
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNMinus : public RPNFunction
{
public:
    RPNMinus() {}
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNMore : public RPNFunction
{
public:
    RPNMore() {}
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNLess : public RPNFunction
{
public:
    RPNLess() {}
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNEquals : public RPNFunction
{
public:
    RPNEquals() {}
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNOr : public RPNFunction
{
public:
    RPNOr() {}
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNAnd : public RPNFunction
{
public:
    RPNAnd() {}
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNSell : public RPNFunction
{
public:
    RPNSell() {}
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNProd : public RPNFunction
{
public:
    RPNProd() {}
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNEndTurn : public RPNFunction
{
public:
    RPNEndTurn() {}
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNBuy : public RPNFunction
{
public:
    RPNBuy() {}
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNBuild : public RPNFunction
{
public:
    RPNBuild() {}
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNRaw : public RPNFunction
{
public:
    RPNRaw() {}
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNMoney : public RPNFunction
{
public:
    RPNMoney() {}
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNTurn : public RPNFunction
{
public:
    RPNTurn() {}
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};


class RPNMyId : public RPNFunction
{
public:
    RPNMyId() {}
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNNOP : public RPNFunction
{
public:
    RPNNOP() {}
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const {return NULL;}
};

class RPNPrint : public RPNFunction
{
public:
    RPNPrint() {}
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

#endif //ROBOT_MANAGER_RPNELEM_H
