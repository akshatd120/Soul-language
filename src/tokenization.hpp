//
// Created by Akshat D. on 15-10-2023.
//

#ifndef SOUL_TOKENIZATION_HPP
#define SOUL_TOKENIZATION_HPP
#include <vector>

enum class TokenType
{
    _variable,
    _if,
    exit,
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

class Tokenizer{
public:
    inline explicit Tokenizer(std::string src)
        :m_src(std::move(src))
    {
    }

    inline std::vector<Token> tokenize()
    {
        std::vector<Token> _tokens;
        std::string buffer;

        while (peek().has_value())
        {
            if(std::isalpha(peek().value()))
            {
                buffer.push_back(consume());
                while (peek().has_value() && std::isalnum(peek().value()))
                {
                    buffer.push_back(consume());
                }
                if(buffer == "exit")
                {
                    _tokens.push_back({.type = TokenType::exit});
                }
                buffer.clear();
            }
            else if(std::isdigit(peek().value()))
            {
                buffer.push_back(consume());
                while (peek().has_value() && std::isdigit(peek().value()))
                {
                    buffer.push_back(consume());
                }
                _tokens.push_back({.type = TokenType::int_lit, .value = buffer});
                buffer.clear();
            }
            else if(peek().value() == ';')
            {
                _tokens.push_back({.type = TokenType::semicolon});
                consume();
            }
            else if (std::isspace(peek().value())){
                consume();
            }
            else {
                std::cerr << "Invalid syntax! :(" << std::endl;
                exit(EXIT_SUCCESS);
            }
        }
        return _tokens;
    }
private:
    [[nodiscard]]inline std::optional<char> peek(int ahead = 1) const {
        if (m_index + ahead > m_src.length()) {
            return {};
        } else {
            return m_src[m_index];
        }
    }
    inline char consume()
    {
        return m_src[m_index++];
    }

    const std::string m_src;
    size_t m_index = 0;
};

#endif //SOUL_TOKENIZATION_HPP
