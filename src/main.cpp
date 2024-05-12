#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include <fstream>
#include <iostream>
#include <string>


#define DEBUG_LEXER true
#define DEBUG_PARSER true
#define DEBUG_LEXER_OUTPUT_PATH "lexer_output.txt"
#define DEBUG_PARSER_OUTPUT_PATH "parser_output.json"

int main(int argc, char *argv[]) {
    // Reading code
    if (argc != 2)
        return 1;

    std::ifstream file(argv[1]);
    if (!file)
        return 1;

    std::string fileContent((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());
    // Lexer
    if (DEBUG_LEXER) {
        std::cout << "=========== Lexer Debug ===========" << std::endl;
        Lexer debugLexer(fileContent);
        if (!std::string(DEBUG_LEXER_OUTPUT_PATH).empty()) {
            std::ofstream file_C(
                DEBUG_LEXER_OUTPUT_PATH,
                std::ios::trunc); // Open file_C in truncate mode
            if (file_C.is_open()) {
                file_C << ""; // Clear file_C
                file_C.close();
            } else {
                std::cout << "Unable to open file_C";
            }
        }
        while (debugLexer.currentChar != "") {
            std::shared_ptr<token::Token> token = debugLexer.nextToken();
            if (!std::string(DEBUG_LEXER_OUTPUT_PATH).empty()) {
                std::ofstream file(DEBUG_LEXER_OUTPUT_PATH,
                                   std::ios::app); // Open file in append mode
                if (file.is_open()) {
                    file << token->toString(false) << std::endl;
                    file.close();
                } else {
                    std::cout << "Unable to open file";
                }
            } else {
                std::cout << token->toString() << std::endl;
            }
        }
    }
    Lexer l(fileContent);
    parser::Parser p(std::make_shared<Lexer>(l));
    if (DEBUG_PARSER) {
        std::cout << "=========== Parser Debug ===========" << std::endl;
        auto program = p.parseProgram();
        if (!std::string(DEBUG_PARSER_OUTPUT_PATH).empty()) {
            std::ofstream file(DEBUG_PARSER_OUTPUT_PATH,
                               std::ios::trunc); // Open file in append mode
            if (file.is_open()) {
                file << program->toJSON()->dump(4) << std::endl;
                file.close();
            } else {
                std::cout << "Unable to open file";
            }
        } else {
            std::cout << program->toJSON()->dump(
                4, ' ', true, nlohmann::json::error_handler_t::replace);
        }
        for (auto &err : p.errors) {
            err->raise(false);
        }
        if (p.errors.size() > 0) {
            return 1;
        }
    }
    return 0;
}