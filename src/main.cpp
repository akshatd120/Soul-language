#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>

#include "tokenization.hpp"

std::string token_to_asm(const std::vector<Token>& tokens)
{
    std::stringstream outStream;
    outStream << "global _start\n_start:\n";
    for (int i = 0; i < tokens.size(); ++i) {
        const Token& token = tokens[i];
        if(token.type == TokenType::exit)
        {
            if(i + 1 < tokens.size() && tokens[i + 1].type == TokenType::int_lit)
            {
                if(i + 2 < tokens.size() && tokens[i + 2].type == TokenType::semicolon)
                {
                    i++;
                    outStream << "    mov rax, 60\n";
                    outStream << "    mov rdi, " << tokens[i].value.value() << '\n';
                    outStream << "    syscall\n";
                }
            }
        }
    }
    return outStream.str();
}

void Assembler_and_Linker(const std::string asm_code,const char* path)
{
    std::cout << asm_code << std::endl;
    {
        std::fstream asmFile(path,std::ios::out);
        asmFile << asm_code;
    }
    system("nasm -felf64 ../test.asm");
    system("ld -o ../out ../test.o");
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Incorrect command usage...\n Correct usage is: \n";
        std::cerr << "soul <filename>.soul <out_filename>.asm";
        return EXIT_FAILURE;
    }
    std::cout << argv[1] << ' ' << argv[2] << '\n';
    std::string soulCode;
    {
        std::stringstream contentStream;
        std::fstream source_file;
        source_file.open(argv[1], std::ios::in);
        contentStream << source_file.rdbuf();
        soulCode = contentStream.str();
        source_file.close();
    }
    Tokenizer s_tokenizer (std::move(soulCode));
    std::vector<Token> s_tokens = s_tokenizer.tokenize();
    Assembler_and_Linker(std::move(token_to_asm(s_tokens)), argv[2]);
    return EXIT_SUCCESS;
}