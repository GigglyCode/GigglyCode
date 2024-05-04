#include <iostream>
#include <fstream>
#include <string>
#include "lexer/lexer.hpp"

bool DEBUG_LEXER = true;

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
        return EXIT_SUCCESS;
    }
}