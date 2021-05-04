#ifndef ROBOT_MANAGER_SYNTAX_H
#define ROBOT_MANAGER_SYNTAX_H

#include <string.h>
#include "../Lex/Lex.h"
#include "ErrorSyntax.h"

class Syntax
{
    lexeme *current_lexeme;
    list *lexeme_list;

    void get_lexeme();

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

    void goto_hdl();

    void condition();

    void if_hdl();

    void check_comma();

    void check_semicolon();

    void keyword_hdl();

    void exp1();

    void cond2();

    void cond1();

    int is_service();

    void service_hdl();

    void check_open_round_bracket();

    void check_open_square_bracket();

    void check_close_square_bracket();

    void check_close_round_bracket();

    void game_func_hdl();

    void statement();

    void multi_statement();
    
    void s();

    void check_open_curly_bracket();

    void check_close_curly_bracket();

    int valid_exp_beginning();

    int is_operand1();

    int is_logic();

    int is_label();

public:

    void start_syntax();

    int check_end() {return lexeme_list == NULL;}

    void load_lexemes(list *lexemes) {lexeme_list = lexemes;}
};


#endif
