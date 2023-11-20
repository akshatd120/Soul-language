//
// Created by Akshat on 20-10-2023.
//

#ifndef SOUL_GENERATION_HPP
#define SOUL_GENERATION_HPP
#include "parser.hpp"

class Generator{
public:
    inline explicit Generator(Node::Program pgm)
    : m_program(std::move(pgm))
    {
    }
    [[nodiscard]]std::string generate_stmt(const Node::Stmt& stmt) const
    {
        std::stringstream out;
        if(holds_alternative<Node::StmtExit>(stmt.s_var))
        {
            out << "    mov rax, 60\n";
            out << "    mov rdi, " << std::get<Node::ExprIntLit>(std::get<Node::StmtExit>(stmt.s_var).expr.e_var).int_lit.val.value() << std::endl;
            out << "    syscall\n";
        }
        return out.str();
    }

    [[nodiscard]]std::string generate_program() const
    {
        std::stringstream generatedString;
        generatedString << "global _start\n _start:\n";

        for(const Node::Stmt& stmt : m_program.stmts)
        {
            generatedString << generate_stmt(stmt);
        }

        generatedString << "    mov rax, 60\n";
        generatedString << "    mov rdi, 0\n";
        generatedString << "    syscall\n";
        return generatedString.str();
    }
private:
    const Node::Program m_program;
};
#endif//SOUL_GENERATION_HPP
