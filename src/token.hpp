//
// Created by Akshat on 20-10-2023.
//

#ifndef SOUL_TOKEN_HPP
#define SOUL_TOKEN_HPP
#include "optional"

enum class TokenType
{
    var,
    _if,
    _exit,
    int_lit,
    semicolon,
    Open_curly_brace,
    Close_curly_brace,
    Open_Circle_brace,
};

struct Token
{
    TokenType type;
    std::optional<std::string> value;
};
#endif//SOUL_TOKEN_HPP
