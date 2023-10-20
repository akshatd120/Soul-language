//
// Created by Akshat on 20-10-2023.
//

#ifndef SOUL_GENERATION_HPP
#define SOUL_GENERATION_HPP
#include "parser.hpp"

class Generator{
public:
    inline explicit Generator(Node::Exit exit_root)
    : m_root(std::move(exit_root))
    {
    }

    [[nodiscard]]std::string generate() const
    {
        std::stringstream generatedString;
        generatedString << "    mov rax, 60\n";
        generatedString << "    mov rdi, " << m_root.expr.int_lit.value.value() << '\n';
        generatedString << "    syscall\n";
        return generatedString.str();
    }
private:
    const Node::Exit m_root;
};
#endif//SOUL_GENERATION_HPP
