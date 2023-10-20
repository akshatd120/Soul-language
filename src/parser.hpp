//
// Created by Akshat on 20-10-2023.
//

#ifndef SOUL_PARSER_HPP
#define SOUL_PARSER_HPP
#include "tokenization.hpp"

namespace Node{
    struct Expr{
        Token int_lit;
    };
    struct Exit{
      Expr expr;
    };
};


class Parser
{
public:
    inline explicit Parser(std::vector<Token> tokens)
    :m_tokens(std::move(tokens))
    {
    }
    std::optional<Node::Expr> parse_expr()
    {
        if(peek().has_value() && peek().value().type == TokenType::int_lit){
            return Node::Expr{.int_lit = consume()};
        }
        return {};
    }
    std::optional<Node::Exit> parse()
    {
        std::optional<Node::Exit> exit_node;
        while (peek().has_value())
        {
            if(peek().value().type == TokenType::_exit)
            {
                consume();
                std::optional<Node::Expr> nodeExpr = parse_expr();
                if(nodeExpr.has_value())
                {
                    exit_node = Node::Exit{.expr = nodeExpr.value() };
                }
                else
                {
                    std::cerr << "Invalid expression" << std::endl;
                    exit(EXIT_FAILURE);
                }
                if (peek().has_value() && peek().value().type == TokenType::semicolon)
                {
                  consume();
                }
                else
                {
                    std::cerr << "Missing a semicolon" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
        }
        return exit_node;
    }
private:
    [[nodiscard]]inline std::optional<Token> peek(int ahead = 1) const {
        if (m_index + ahead > m_tokens.size()) {
            return {};
        } else {
            return m_tokens[m_index];
        }
    }
    inline Token consume()
    {
        return m_tokens[m_index++];
    }
    std::vector<Token> m_tokens;
    size_t m_index = 0;
};
#endif//SOUL_PARSER_HPP
