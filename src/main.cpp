#include <fstream>
#include "include/lexer.hpp"
#include "include/parser.hpp"

const bool DEBUG_LEXER = false;
const bool DEBUG_PARSER = true;

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
        std::cout << "=========== Lexer Debug =========== \n";
        Lexer debugLexer = fileContent;
        do
        {
            std::shared_ptr<Token> token(debugLexer.nextToken());
            token->print();
        } while (debugLexer.currentChar != "\0");
    }
    std::shared_ptr<Lexer> l = std::make_shared<Lexer>(fileContent);
    Parser p = Parser(l);
    if (DEBUG_PARSER)
    {
        std::cout << "=========== Parser Debug =========== \n";
        std::shared_ptr<AST::Program> program = p.parseProgram();
        auto json = program->toJson();
        std::ofstream outputFile(".\\dump\\parser.json");
        outputFile << json->dump(4) << std::endl;
        outputFile.close();
        std::cout << "Json dumped to output.json" << std::endl;
    }
    return 0;
}
