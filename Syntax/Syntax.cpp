#include "Syntax.h"

void Syntax::start_syntax()
{
    while (!check_end())
    {
        get_lexeme();
        s();
    }
}

void Syntax::s()
{
    if (is_statement())
        statement();
    else
        throw ErrorSyntax(current_lexeme->line,
      "Invalid token. Operator or label expected", current_lexeme->name);
}

void Syntax::multi_statement()
{
    if (is_var())
    {
        add_rpn(&rpn_list, new RPNVarAddr(current_lexeme->name));
        get_lexeme();
        init_hdl();
        get_lexeme();
        add_rpn(&rpn_list, new RPNAssign);
    }
    else if (is_key_word())
    {
        keyword_hdl();
        get_lexeme();
    }
    else if (is_function())
    {
        game_func_hdl();
        get_lexeme();
        check_semicolon();
        get_lexeme();
    }
    if (!is_bracket())
        multi_statement();
}

void Syntax::statement()
{
    if (is_var())
    {
        add_rpn(&rpn_list, new RPNVarAddr(current_lexeme->name));
        get_lexeme();
        init_hdl();
        add_rpn(&rpn_list, new RPNAssign);
    }
    else if (is_key_word())
        keyword_hdl();
    else if (is_function())
    {
        game_func_hdl();
        get_lexeme();
        check_semicolon();
    }
    else
        throw ErrorSyntax(current_lexeme->line,
          "Invalid token. Operator expected", current_lexeme->name);
}

void Syntax::game_func_hdl()
{
    char *tmp_lex_name = current_lexeme->name;
    if (lex_equals("money") || lex_equals("raw"))
    {
        get_lexeme();
        check_open_round_bracket();
        get_lexeme();
        exp();
        check_close_round_bracket();
        if (str_equals(tmp_lex_name, "money"))
            add_rpn(&rpn_list, new RPNMoney);
        else
            add_rpn(&rpn_list, new RPNRaw);
    }
    else if (lex_equals("my_id"))
            add_rpn(&rpn_list, new RPNMyId);
    else if (lex_equals("turn"))
            add_rpn(&rpn_list, new RPNTurn);
    else
        throw ErrorSyntax(current_lexeme->line,
        "Unknown game command", current_lexeme->name);
}

void Syntax::check_close_round_bracket()
{
    if (!(is_bracket() && lex_equals(")")))
        throw ErrorSyntax(current_lexeme->line,
          "Invalid syntax. ) expected", current_lexeme->name);
}

void Syntax::check_close_square_bracket()
{
    if (!(is_bracket() && lex_equals("]")))
        throw ErrorSyntax(current_lexeme->line,
          "Invalid syntax. ] expected", current_lexeme->name);
}

//void Syntax::check_open_square_bracket()
//{
//    if (!(is_bracket() && lex_equals("[")))
//        throw ErrorSyntax(current_lexeme->line,
//          "Invalid syntax. [ expected", current_lexeme->name);
//}

void Syntax::exp1()
{
    if (is_bracket() && lex_equals("["))
    {
        get_lexeme();
        exp();
        check_close_square_bracket();
        get_lexeme();
    }
}

int Syntax::is_operand1()
{
    return is_num() || is_var() || is_function() || is_str();
}

void Syntax::exp()
{
    exp2();
    if (lex_equals("or"))
    {
        get_lexeme();
        exp();
        add_rpn(&rpn_list, new RPNOr);
    }
}

void Syntax::exp2()
{
    exp3();
    if (lex_equals("and"))
    {
        get_lexeme();
        exp2();
        add_rpn(&rpn_list, new RPNAnd);
    }
}

void Syntax::exp3()
{
    exp4();
    if (lex_equals(">"))
    {
        get_lexeme();
        exp3();
        add_rpn(&rpn_list, new RPNMore);
    }
    else if (lex_equals("<"))
    {
        get_lexeme();
        exp3();
        add_rpn(&rpn_list, new RPNLess);
    }

    else if (lex_equals("="))
    {
        get_lexeme();
        exp3();
        add_rpn(&rpn_list, new RPNEquals);
    }
}

void Syntax::exp4()
{
    exp5();
    if (lex_equals("+"))
    {
        get_lexeme();
        exp4();
        add_rpn(&rpn_list, new RPNFunPlus);
    }
    else if (lex_equals("-"))
    {
        get_lexeme();
        exp4();
        add_rpn(&rpn_list, new RPNFunMinus);
    }
}

void Syntax::exp5()
{
    exp6();
    if (lex_equals("*"))
    {
        get_lexeme();
        exp5();
        add_rpn(&rpn_list, new RPNFunMultiply);
    }
    else if (lex_equals("/"))
    {
        get_lexeme();
        exp5();
        add_rpn(&rpn_list, new RPNFunDivision);
    }

    else if (lex_equals("%"))
    {
        get_lexeme();
        exp5();
        add_rpn(&rpn_list, new RPNFunModulo);
    }
}

void Syntax::exp6()
{
    if (lex_equals("!"))
    {
        get_lexeme();
        exp6();
        add_rpn(&rpn_list, new RPNNot);
    }
    else if (lex_equals("-"))
    {
        get_lexeme();
        exp6();
        add_rpn(&rpn_list, new RPNMinus);
    }
    else
        exp7();
}

void Syntax::exp7()
{
    if (is_bracket())
    {
        check_open_round_bracket();
        get_lexeme();
        exp();
        check_close_round_bracket();
        get_lexeme();
    }
    else if (is_function())
    {
        game_func_hdl();
        get_lexeme();
    }
    else if (!(lex_equals("-") || lex_equals("!")))
    {
        if (!is_operand1())
            throw ErrorSyntax(current_lexeme->line,
              "Invalid operand. Const, func or var expected", NULL);
        if (is_num())
            add_rpn(&rpn_list, new RPNInt(atoi(current_lexeme->name)));
        else if (is_var())
        {
            add_rpn(&rpn_list, new RPNVarAddr(current_lexeme->name));
            add_rpn(&rpn_list, new RPNVar);
        }
        else if (is_str())
            add_rpn(&rpn_list, new RPNString(current_lexeme->name));
        get_lexeme();
        exp1();
    }
}

void Syntax::get_lexeme()
{
    if (check_end())
        throw ErrorSyntax(current_lexeme->line,
          "Invalid syntax. ; expected", NULL);
    current_lexeme = lexeme_list->l;
    lexeme_list = lexeme_list->next;
}

int Syntax::is_bracket()
{
    return current_lexeme->type == brackets;
}

void Syntax::init_hdl()
{
    if (is_equal())
    {
        get_lexeme();
        exp();
        check_semicolon();
    }
    else
        throw ErrorSyntax(current_lexeme->line,
        "Invalid syntax. := expected", current_lexeme->name);
}

void Syntax::if_hdl()
{
    RPNLabel *lab = new RPNLabel;
    check_open_round_bracket();
    get_lexeme();
    exp();
    add_rpn(&rpn_list, lab);
    add_rpn(&rpn_list, new RPNOpGOFalse);
    check_close_round_bracket();
    get_lexeme();
    check_open_curly_bracket();
    get_lexeme();
    multi_statement();
    check_close_curly_bracket();
    add_rpn(&rpn_list, new RPNNOP);
    lab->set(get_last_elem());
}

void Syntax::while_hdl()
{
    RPNItem *addr1;
    RPNLabel *lab1 = new RPNLabel;
    check_open_round_bracket();
    add_rpn(&rpn_list, new RPNNOP);
    addr1 = get_last_elem();
    get_lexeme();
    exp();
    add_rpn(&rpn_list, lab1);
    add_rpn(&rpn_list, new RPNOpGOFalse);
    check_close_round_bracket();
    get_lexeme();
    check_open_curly_bracket();
    get_lexeme();
    multi_statement();
    check_close_curly_bracket();
    add_rpn(&rpn_list, new RPNLabel(addr1));
    add_rpn(&rpn_list, new RPNOpGO);
    add_rpn(&rpn_list, new RPNNOP);
    lab1->set(get_last_elem());
}

void Syntax::check_comma()
{
    if (!(current_lexeme->type == delimiter && lex_equals(",")))
        throw ErrorSyntax(current_lexeme->line,
      "Invalid syntax. , expected", current_lexeme->name);
}

void Syntax::check_semicolon()
{
    if (!lex_equals(";"))
        throw ErrorSyntax(current_lexeme->line,
      "Invalid syntax. ; expected", current_lexeme->name);
}

int Syntax::is_service()
{
    return lex_equals("buy") || lex_equals("sell")
    || lex_equals("build") || lex_equals("prod")
    || lex_equals("endturn") || lex_equals("print")
    || lex_equals("println");
}

void Syntax::service_hdl()
{
    char *tmp_lex_name = current_lexeme->name;
    if (lex_equals("buy") || lex_equals("sell"))
    {
        get_lexeme();
        exp();
        check_comma();
        get_lexeme();
        exp();
        if (str_equals(tmp_lex_name, "buy"))
            add_rpn(&rpn_list, new RPNBuy);
        else if (str_equals(tmp_lex_name, "sell"))
            add_rpn(&rpn_list, new RPNSell);
        check_semicolon();
    }
    else if (lex_equals("build") || lex_equals("prod")
    || lex_equals("print"))
    {
        get_lexeme();
        exp();
        check_semicolon();
        if (str_equals(tmp_lex_name, "build"))
            add_rpn(&rpn_list, new RPNBuild);
        else if (str_equals(tmp_lex_name, "sell"))
            add_rpn(&rpn_list, new RPNSell);
        else if (str_equals(tmp_lex_name, "print"))
            add_rpn(&rpn_list, new RPNPrint);
    }
    else if (lex_equals("endturn"))
    {
        add_rpn(&rpn_list, new RPNEndTurn);
        get_lexeme();
        check_semicolon();
    }
}

void Syntax::keyword_hdl()
{
    if (lex_equals("while"))
    {
        get_lexeme();
        while_hdl();
    }
    else if (lex_equals("if"))
    {
        get_lexeme();
        if_hdl();
    }
    else if (is_service())
        service_hdl();
    else
        throw ErrorSyntax(current_lexeme->line,
      "Unknown operator", current_lexeme->name);
}

void Syntax::check_open_round_bracket()
{
    if (!(current_lexeme->type == brackets && lex_equals("(")))
        throw ErrorSyntax(current_lexeme->line,
          "Invalid syntax. ( expected", current_lexeme->name);
}

void Syntax::check_open_curly_bracket()
{
    if (!(current_lexeme->type == brackets && lex_equals("{")))
        throw ErrorSyntax(current_lexeme->line,
          "Invalid syntax. { expected", current_lexeme->name);
}

void Syntax::check_close_curly_bracket()
{
    if (!(current_lexeme->type == brackets && lex_equals("}")))
        throw ErrorSyntax(current_lexeme->line,
          "Invalid syntax. } expected", current_lexeme->name);
}

RPNItem *Syntax::get_last_elem()
{
    RPNItem *tmp = rpn_list;
    if (tmp == NULL)
        return tmp;
    while (tmp->next != NULL)
        tmp = tmp->next;
    return tmp;
}

void Syntax::add_rpn(RPNItem **list, RPNElem *el)
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
