//
// Created by Akshat on 20-10-2023.
//

#ifndef SOUL_PARSER_HPP
#define SOUL_PARSER_HPP

#include <variant>

#include "tokenization.hpp"

namespace Node{
    struct ExprIntLit{
        Token int_lit;
    };
    struct ExprIdent{
        Token ident;
    };
    struct Expr{
        std::variant<ExprIntLit , ExprIdent> e_var;
    };
    struct Exit{
      Expr expr;
    };
    struct StmtExit{
        Expr expr;
    };
    struct StmtVar{
        Token ident;
        Expr expr;
    };
    struct Stmt{
        std::variant <StmtExit, StmtVar> s_var;
    };
    struct Program{
        std::vector<Stmt> stmts;
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
        if(peek().has_value())
        {
            if(peek().value().type == TokenType::int_lit)
            {
                return Node::Expr{ .e_var = Node::ExprIntLit{.int_lit = consume()} };
            }
            else if(peek().value().type == TokenType::ident)
            {
                return Node::Expr{.e_var = Node::ExprIdent{.ident = consume()}};
            }
        }
        return {};
    }
    std::optional<Node::Stmt> parse_stmt()
    {
        if(peek().has_value() && peek().value().type == TokenType::exit)
        {
            //consume exit
            consume();
            if(peek().has_value() && peek().value().type == TokenType::open_paren)
            {
                consume();
            }
            else
            {
                std::cerr << "Expected `(`" << std::endl;
                exit(EXIT_FAILURE);
            }
            //Parse the Expr with int lit
            Node::StmtExit stmt_exit;
            if(auto node_expr = parse_expr())
            {
                stmt_exit = {.expr = node_expr.value()};
            }
            else
            {
                std::cerr << "Invalid Expression..." << std::endl;
                exit(EXIT_FAILURE);
            }
            if(peek().has_value() && peek().value().type == TokenType::close_paren)
            {
                consume();
            }
            else
            {
                std::cerr << "Expected `)`" << std::endl;
                exit(EXIT_FAILURE);
            }
            if(peek().has_value() && peek().value().type == TokenType::semicolon) //IF it is a semicolon
            {
                //consume semicolon
                consume();
            }
            else //NO Semicolon found
            {
                std::cerr << "Expected `;`" << std::endl;
                exit(EXIT_FAILURE);
            }


            return Node::Stmt{.s_var = stmt_exit};
        }
        else if(peek().has_value() && peek().value().type == TokenType::var
                 && peek(1).has_value() && peek(1).value().type == TokenType::ident)
        {
            //consume var & ident but store ident
            consume();
            Node::StmtVar stmt_var;
            stmt_var = {.ident = consume()};

            if(peek().has_value() && peek().value().type == TokenType::equalSign)
            {
                consume();
            }
            else{
                std::cerr << "Expected `=`" << std::endl;
                exit(EXIT_FAILURE);
            }
            if(auto node_expr = parse_expr())
            {
                stmt_var.expr = node_expr.value();
            }
            else
            {
                std::cerr << "Invalid Expression." << std::endl;
                exit(EXIT_FAILURE);
            }
            if(peek().has_value() && peek().value().type == TokenType::semicolon) //IF it is a semicolon
            {
                //consume semicolon
                consume();
            }
            else //NO Semicolon found
            {
                std::cerr << "Expected `;`" << std::endl;
                exit(EXIT_FAILURE);
            }

            return Node::Stmt{.s_var = stmt_var};
        }
        else
            return {}; // Return nothing, coz parsing failed/ found ntg
    }
    std::optional<Node::Program> parse_program ()
    {
        Node::Program pgm;
        while (peek().has_value())
        {
            if(auto node_stmt = parse_stmt())
            {
                pgm.stmts.push_back(node_stmt.value());
            }
            else
            {
                std::cerr << "Invalid statement" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        return pgm;
    }

private:
    [[nodiscard]]inline std::optional<Token> peek(int ahead = 0) const {
        if (m_index + ahead >= m_tokens.size()) {
        return {};
        } else {
        return m_tokens[m_index + ahead];
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
