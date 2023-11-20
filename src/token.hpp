//
// Created by Akshat on 20-10-2023.
//

#ifndef SOUL_TOKEN_HPP
#define SOUL_TOKEN_HPP
#include "optional"
enum class TokenType
{
    var,
    exit,
    int_lit,
    ident,
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
