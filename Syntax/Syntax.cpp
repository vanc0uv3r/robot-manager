#include "Syntax.h"

void Syntax::start_syntax()
{
    while (!check_end())
    {
        get_lexeme();
        s();
    }
}

int Syntax::is_label()
{
    return current_lexeme->type == label;
}

void Syntax::s()
{
    if (is_statement())
        statement();
    else if (!is_label())
        throw ErrorSyntax(current_lexeme->line,
          "Invalid token. Operator or label expected", current_lexeme->name);
}

void Syntax::multi_statement()
{
    if (is_var())
    {
        get_lexeme();
        init_hdl();
        get_lexeme();
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
    if (!is_bracket() && !is_label())
        multi_statement();
}

void Syntax::statement()
{
    if (is_var())
    {
        get_lexeme();
        init_hdl();
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
    if (lex_equals("money") || lex_equals("raw"))
    {
        get_lexeme();
        check_open_round_bracket();
        get_lexeme();
        exp();
        check_close_round_bracket();
    }
    else if (!(lex_equals("my_id") || lex_equals("turn")))
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

void Syntax::check_open_square_bracket()
{
    if (!(is_bracket() && lex_equals("[")))
        throw ErrorSyntax(current_lexeme->line,
          "Invalid syntax. [ expected", current_lexeme->name);
}

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

void Syntax::cond2()
{
    if (is_logic())
    {
        get_lexeme();
        cond1();
    }
}

void Syntax::cond1()
{
    if (is_bracket())
    {
        check_open_round_bracket();
        get_lexeme();
        cond1();
        check_close_round_bracket();
        get_lexeme();
    }
    else
        exp();
    cond2();
}

int Syntax::valid_exp_beginning()
{
    return is_str() || is_operand1() || lex_equals("(")
    || lex_equals("!");
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
        // add
    }
}

void Syntax::exp2()
{
    exp3();
    if (lex_equals("and"))
    {
        get_lexeme();
        exp2();
        // add
    }
}

void Syntax::exp3()
{
    exp4();
    if (lex_equals(">"))
    {
        get_lexeme();
        exp3();
        // add
    }
    else if (lex_equals("<"))
    {
        get_lexeme();
        exp3();
        // add
    }

    else if (lex_equals("="))
    {
        get_lexeme();
        exp3();
        // add
    }
}

void Syntax::exp4()
{
    exp5();
    if (lex_equals("+"))
    {
        get_lexeme();
        exp4();
        // add
    }
    else if (lex_equals("-"))
    {
        get_lexeme();
        exp4();
        // add
    }
}

void Syntax::exp5()
{
    exp6();
    if (lex_equals("*"))
    {
        get_lexeme();
        exp5();
        // add
    }
    else if (lex_equals("/"))
    {
        get_lexeme();
        exp5();
        // add
    }

    else if (lex_equals("%"))
    {
        get_lexeme();
        exp5();
        // add
    }
}

void Syntax::exp6()
{
    exp7();
    if (lex_equals("!"))
    {
        get_lexeme();
        exp6();
        // add
    }
    else if (lex_equals("-"))
    {
            get_lexeme();
            exp6();
            // add
    }
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
    else if (is_bracket())
    {
        check_open_square_bracket();
        get_lexeme();
        exp();
        check_close_square_bracket();
        get_lexeme();
        if (is_equal())
        {
            get_lexeme();
            exp();
        }
        check_semicolon();
    }
    else
        throw ErrorSyntax(current_lexeme->line,
          "Invalid syntax. := expected", current_lexeme->name);
}

void Syntax::goto_hdl()
{
    if (current_lexeme->type != label)
        throw ErrorSyntax(current_lexeme->line,
          "Invalid syntax. label expected", current_lexeme->name);
}

int Syntax::is_logic()
{
    return (lex_equals("and") || lex_equals("or"))
    && current_lexeme->type == key_word;
}

void Syntax::condition()
{
    get_lexeme();
    cond1();
}

void Syntax::if_hdl()
{
    check_open_round_bracket();
    condition();
    check_close_round_bracket();
    get_lexeme();
    check_open_curly_bracket();
    get_lexeme();
    multi_statement();
    check_close_curly_bracket();
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
    || lex_equals("build") || lex_equals("prod") || lex_equals("endturn");
}

void Syntax::service_hdl()
{
    if (lex_equals("buy") || lex_equals("sell"))
    {
        get_lexeme();
        exp();
        check_comma();
        get_lexeme();
        exp();
        check_semicolon();
    }
    else if (lex_equals("build") || lex_equals("prod"))
    {
        get_lexeme();
        exp();
        check_semicolon();
    }
    else if (lex_equals("endturn"))
    {
        get_lexeme();
        check_semicolon();
    }
}


void Syntax::keyword_hdl()
{
    if (lex_equals("goto"))
    {
        get_lexeme();
        goto_hdl();
        get_lexeme();
        check_semicolon();
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