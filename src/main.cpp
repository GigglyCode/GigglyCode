#include <iostream>
#include <fstream>
#include <string>
#include "lexer/lexer.hpp"
#include "parser/parser.hpp"

bool DEBUG_LEXER = false;
bool DEBUG_PARSER = true;

int main(int argc, char *argv[])
{
    // Reading code
    if (argc != 2)
        return 1;

    std::ifstream file(argv[1]);
    if (!file)
        return 1;

    std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    // Lexer
    if (DEBUG_LEXER)
    {
        std::cout << "=========== Lexer Debug ===========" << std::endl;
        Lexer debugLexer(fileContent);
        while (debugLexer.currentChar != "")
        {
            std::shared_ptr<token::Token> token = debugLexer.nextToken();
            token->print();
        }
    }
    Lexer l(fileContent);
    parser::Parser p(std::make_shared<Lexer>(l));
    if (DEBUG_PARSER)
    {
        std::cout << "=========== Parser Debug ===========" << std::endl;
        auto program = p.parseProgram();
        std::cout << program->toJSON()->dump(4);
        for (auto &err : p.errors)
        {
            err->raise(false);
        }
        if (p.errors.size() > 0)
        {
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}