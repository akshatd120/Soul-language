//
// Created by Akshat D. on 15-10-2023.
//

#ifndef SOUL_TOKENIZATION_HPP
#define SOUL_TOKENIZATION_HPP
#include <vector>
#include "token.hpp"

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
                    _tokens.emplace_back(TokenType::exit);
                }
                else if(buffer == "var")
                {
                    _tokens.emplace_back(TokenType::var);
                }
                else
                {
                    _tokens.emplace_back(TokenType::ident,buffer);
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
                _tokens.emplace_back(TokenType::int_lit,buffer);
                buffer.clear();
            }
            else if(peek().value() == '(')
            {
                _tokens.emplace_back(TokenType::open_paren);
                consume();
            }
            else if(peek().value() == '=')
            {
                _tokens.emplace_back(TokenType::equalSign);
                consume();
            }
            else if (peek().value() == ')')
            {
                _tokens.emplace_back(TokenType::close_paren);
                consume();
            }
            else if(peek().value() == ';')
            {
                _tokens.emplace_back(TokenType::semicolon);
                consume();
            }
            else if(peek().value() == '/')
            {
                consume();
                //Check: For single line comment
                if(peek().has_value() && peek().value() == '/')
                {
                    consume();
                    while(peek().has_value() && peek().value() != '\n')
                    {
                        consume();
                    }
                    //consume the newline character
                }
                else if(peek().has_value() && peek().value() == '*')
                {
                    consume();
                    while (peek().has_value())
                    {
                        if(peek().value() != '*' && peek(1).has_value() && peek(1).value() == '/')
                        {
                            consume();
                            consume();
                            break;
                        }
                        consume();
                    }
                }
                else
                {
                    std::cerr << "Invalid syntax!" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
            else if (std::isspace(peek().value()))
            {
                consume();
            }
            else
            {
                std::cerr << "Invalid syntax!" << std::endl;
                exit(EXIT_SUCCESS);
            }
        }
        return _tokens;
    }
private:
    [[nodiscard]]inline std::optional<char> peek(int ahead = 0) const {
        if (m_index + ahead >= m_src.length()) {
            return {};
        } else {
            return m_src[m_index + ahead];
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
