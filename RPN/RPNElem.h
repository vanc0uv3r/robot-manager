#ifndef ROBOT_MANAGER_RPNELEM_H
#define ROBOT_MANAGER_RPNELEM_H

#include <string.h>

class RPNElem;

struct RPNItem
{
    RPNElem *el;
    RPNItem *next;
};

class RPNElem
{
public:
    virtual void evaluate(RPNItem **stack, RPNItem **cur_cmd) const = 0;
    virtual ~RPNElem() {}
protected:
    static void push(RPNItem **stack, RPNElem *el);
    static RPNElem *pop(RPNItem **stack);
};

class RPNConstant : public RPNElem
{
public:
    virtual RPNElem *clone() const = 0;
    void evaluate(RPNItem **stack, RPNItem **cur_cmd) const;
};

class RPNFunction : public RPNElem
{
public:
    void evaluate(RPNItem **stack, RPNItem **cur_cmd) const;
    virtual RPNElem *evaluate_fun(RPNItem **stack) const = 0;
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
    ~RPNString() {delete[] value;}
};

class RPNLabel : public RPNConstant
{
    RPNElem *value;
public:
    RPNLabel(RPNElem *el);
    RPNElem *clone() const;
    RPNElem *get() const;
    ~RPNLabel() {}
};

class RPNVarAddr : public RPNConstant
{
    RPNElem *value;
public:
    RPNVarAddr(RPNElem *a);
    RPNElem *clone() const;
    RPNElem *get() const;
    ~RPNVarAddr() {}
};

class RPNFunPlus : public RPNFunction
{
public:
    ~RPNFunPlus() {}
    RPNElem *evaluate_fun(RPNItem **stack) const;
};

class RPNFunMinus : public RPNFunction
{
public:
    ~RPNFunMinus() {}
    RPNElem *evaluate_fun(RPNItem **stack) const;
};



#endif //ROBOT_MANAGER_RPNELEM_H
