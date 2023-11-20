#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>

#include "tokenization.hpp"
#include "parser.hpp"
#include "generation.hpp"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Incorrect command usage...\n Correct usage is: \n";
        std::cerr << "soul <filename>.soul <out_filename>.asm";
        return EXIT_FAILURE;
    }
    std::cout << argv[1] << ' ' << argv[2] << '\n';

    //File reading
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

    Parser parser(std::move(s_tokens));
    std::optional<Node::Program> parser_out = parser.parse_program();

    if(!parser_out.has_value()) {
        std::cerr << "No exit found" << std::endl;
        exit(EXIT_FAILURE);
    }
    Generator generator (parser_out.value());

    //NASM and Linking
    {
        std::fstream asmFile(argv[2],std::ios::out);
        asmFile << generator.generate_program();
    }
    system("nasm -felf64 ../test.asm");
    system("ld -o ../out ../test.o");

    return EXIT_SUCCESS;
}