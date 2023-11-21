//
// Created by Akshat on 20-10-2023.
//

#ifndef SOUL_TOKEN_HPP
#define SOUL_TOKEN_HPP
#include "optional"
enum class TokenType
{
    //Keywords
    var,
    exit,
    println,

    //Lits & Idents
    str_lit,
    int_lit,
    ident,

    //Symbols
    s_doubleQuote,
    e_doubleQuote,
    equalSign,
    open_paren,
    close_paren,
    semicolon
};

struct Token
{
    TokenType type;
    std::optional<std::string> val;
};
#endif//SOUL_TOKEN_HPP
