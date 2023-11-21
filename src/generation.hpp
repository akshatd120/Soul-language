//
// Created by Akshat on 20-10-2023.
//

#ifndef SOUL_GENERATION_HPP
#define SOUL_GENERATION_HPP

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <numeric>

#include "parser.hpp"

std::string gen_random(const int len) {
    static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len + 1);
    tmp_s += 's';

    for (int i = 1; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return tmp_s;
}


class Generator{
public:
    inline explicit Generator(Node::Program pgm)
    : m_program(std::move(pgm))
    {
    }

    [[nodiscard]]std::string generate_stmt(const Node::Stmt& stmt)
    {
        std::stringstream out;
        if(holds_alternative<Node::StmtExit>(stmt.s_var))
        {
            out << "    mov rax, 60\n";
            out << "    mov rdi, " << std::get<Node::ExprIntLit>(std::get<Node::StmtExit>(stmt.s_var).expr.e_var).int_lit.val.value() << std::endl;
            out << "    syscall\n";
        }
        else if(holds_alternative<Node::StmtPrint>(stmt.s_var))
        {
            out << "    mov rax, 1\n";
            out << "    mov rdi, 1\n";

            std::string label_name = gen_random(5);
            std::string dataStr = std::get<Node::ExprStrLit>(std::get<Node::StmtPrint>(stmt.s_var).expr.e_var).str_lit.val.value();
            AddROString(label_name,dataStr);

            out << "    mov rsi, " << label_name << std::endl;
            out << "    mov rdx, " << label_name << "len\n";
            out << "    syscall\n";
        }
        return out.str();
    }

    [[nodiscard]]std::string generate_program()
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
        generatedString << GenerateROString();
        return generatedString.str();
    }
private:
   [[nodiscard]] inline std::string GenerateROString() const {
       std::stringstream SectionROdata;
       SectionROdata << "section .rodata\n";

       for(const auto & ROData : roDataContent)
            SectionROdata << ROData;

       return SectionROdata.str();
   }
   void AddROString(std::string& label, std::string& val)
   {
       std::stringstream ROString;
       ROString << label << ": db \"" << val <<"\", 10" << std::endl;
       ROString << label << "len: equ $ - " << label << std::endl;
       roDataContent.emplace_back(std::move(ROString.str()));
   }

    const Node::Program m_program;
    std::vector <std::string> roDataContent;
};
#endif//SOUL_GENERATION_HPP
