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
    virtual void evaluate(RPNItem **stack, RPNItem **cur_cmd, 
    var_list **vars)
    const = 0;
    virtual ~RPNElem() {}
    virtual const char *getPRNName() = 0;
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
    const char *name;
public:
    RPNInt(int a);
    RPNElem *clone() const;
    const char * getPRNName();
    int get() const;
    ~RPNInt() {}
};

class RPNString : public RPNConstant
{
    char *value;
public:
    RPNString(char *s);
    RPNElem *clone() const;
    char *getPRNName();
    char *get() const;
    ~RPNString() {delete value;}
};

class RPNLabel : public RPNConstant
{
    RPNItem *value;
public:
    RPNLabel(RPNItem *a = NULL);
    RPNElem *clone() const;
    const char * getPRNName();
    RPNItem *get() const;
    ~RPNLabel() {}
    void set(RPNItem *addr) {value = addr;}
};

class RPNVarAddr : public RPNConstant
{
    char *value;
public:
    RPNVarAddr(char *a);
    RPNElem *clone() const;
    const char * getPRNName();
    char *get() const;
    ~RPNVarAddr() {delete value;}
};

class RPNFunPlus : public RPNFunction
{
public:
    ~RPNFunPlus() {}
    const char * getPRNName();
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNFunMinus : public RPNFunction
{
public:
    ~RPNFunMinus() {}
    const char * getPRNName();
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNFunMultiply : public RPNFunction
{
public:
    ~RPNFunMultiply() {}
    const char * getPRNName();
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNFunModulo : public RPNFunction
{
public:
    ~RPNFunModulo() {}
    const char * getPRNName();
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNFunDivision : public RPNFunction
{
public:
    ~RPNFunDivision() {}
    const char * getPRNName();
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNOpGO : public RPNElem
{
public:
    ~RPNOpGO() {}
    const char *getPRNName();
    void evaluate(RPNItem **stack, RPNItem **cur_cmd, var_list **vars) const;
};

class RPNOpGOFalse : public RPNElem
{
public:
    ~RPNOpGOFalse() {}
    const char *getPRNName();
    void evaluate(RPNItem **stack, RPNItem **cur_cmd, var_list **vars) const;
};

class RPNVar : public RPNFunction
{
public:
    RPNVar() {}
    const char *getPRNName();
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNAssign : public RPNFunction
{
public:
    RPNAssign() {}
    const char *getPRNName();
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNNot : public RPNFunction
{
public:
    RPNNot() {}
    const char *getPRNName();
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNMinus : public RPNFunction
{
public:
    RPNMinus() {}
    const char *getPRNName();
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNMore : public RPNFunction
{
public:
    RPNMore() {}
    const char * getPRNName();
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNLess : public RPNFunction
{
public:
    RPNLess() {}
    const char *getPRNName();
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNEquals : public RPNFunction
{
public:
    RPNEquals() {}
    const char * getPRNName();
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNOr : public RPNFunction
{
public:
    RPNOr() {}
    const char *getPRNName();
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNAnd : public RPNFunction
{
public:
    RPNAnd() {}
    const char *getPRNName();
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNSell : public RPNFunction
{
public:
    RPNSell() {}
    const char *getPRNName();
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNProd : public RPNFunction
{
public:
    RPNProd() {}
    const char *getPRNName() {return "prod";}
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNEndTurn : public RPNFunction
{
public:
    RPNEndTurn() {}
    const char *getPRNName() {return "endturn";}
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNBuy : public RPNFunction
{
public:
    RPNBuy() {}
    const char *getPRNName() {return "buy";}
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNBuild : public RPNFunction
{
public:
    RPNBuild() {}
    const char *getPRNName() {return "build";}
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNRaw : public RPNFunction
{
public:
    RPNRaw() {}
    const char *getPRNName() {return "raw";}
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNMoney : public RPNFunction
{
public:
    RPNMoney() {}
    const char *getPRNName() {return "money";}
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNTurn : public RPNFunction
{
public:
    RPNTurn() {}
    const char *getPRNName() {return "turn";}
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};


class RPNMyId : public RPNFunction
{
public:
    RPNMyId() {}
    const char *getPRNName();
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

class RPNNOP : public RPNFunction
{
public:
    RPNNOP() {}
    const char *getPRNName();
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const {return NULL;}
};

class RPNPrint : public RPNFunction
{
public:
    RPNPrint() {}
    const char *getPRNName();
    RPNElem *evaluate_fun(RPNItem **stack, var_list **vars) const;
};

#endif //ROBOT_MANAGER_RPNELEM_H
