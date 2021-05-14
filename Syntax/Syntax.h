#ifndef ROBOT_MANAGER_SYNTAX_H
#define ROBOT_MANAGER_SYNTAX_H

#include <string.h>
#include "../Lex/Lex.h"
#include "ErrorSyntax.h"
#include "../RPN/RPNElem.h"

class Syntax
{
    lexeme *current_lexeme;
    list *lexeme_list;
    RPNItem *rpn_list;

    void get_lexeme();

    void add_rpn(RPNItem **list, RPNElem *el)
    {
        if (*list == NULL)
        {
            *list = new RPNItem;
            (*list)->el = el;
            (*list)->next = NULL;
        }
        else
            add_rpn(&((*list)->next), el);
    }

    int is_bracket();

    int lex_equals(const char *str) {return !strcmp(current_lexeme->name, str);}

    int is_function() {return current_lexeme->type == function;}

    int is_num() {return current_lexeme->type == num;}

    int is_var() {return current_lexeme->type == variable;}

    int is_key_word() {return current_lexeme->type == key_word;}

    int is_equal() {return current_lexeme->type == equal;}

    int is_str() {return current_lexeme->type == str_const;}

    int is_statement() {return is_var() || is_key_word() || is_function();}

    void exp();

    void init_hdl();

    RPNItem *get_last_elem();

    void if_hdl();

    void check_comma();

    void check_semicolon();

    void keyword_hdl();

    void exp1();

    void exp2();

    void exp3();

    void exp4();

    void exp5();

    void exp6();

    void exp7();

    void cond2();

    void cond1();

    int is_service();

    void service_hdl();

    void check_open_round_bracket();

    void check_close_square_bracket();

    void check_close_round_bracket();

    void game_func_hdl();

    void statement();

    void multi_statement();
    
    void s();

    void check_open_curly_bracket();

    void check_close_curly_bracket();

    int is_operand1();

    int is_logic();

    int is_label();

public:

    void start_syntax();

    int check_end() {return lexeme_list == NULL;}

    void load_lexemes(list *lexemes) {lexeme_list = lexemes;}

    Syntax() {rpn_list = NULL;}

    void while_hdl();

    RPNItem *get_rpn() {return rpn_list;}
};


#endif
