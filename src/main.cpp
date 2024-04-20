#include "include/lexer.hpp"

const bool DEBUG_LEXER = true;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Please provide a file path as a command-line parameter." << std::endl;
        return 1;
    }

    std::string filePath = argv[1];
    std::ifstream file(filePath);

    if (!file)
    {
        std::cout << "Failed to open the file." << std::endl;
        return 1;
    }

    std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    if (DEBUG_LEXER)
    {
        Lexer debugLexer = fileContent;
        do
        {
            Token *token = debugLexer.nextToken();
            token->print();
        } while (debugLexer.currentChar != "\0");
    }

    return 0;
}
