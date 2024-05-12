#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include <fstream>
#include <iostream>
#include <string>

#define DEBUG_LEXER true
#define DEBUG_PARSER true
#define DEBUG_LEXER_OUTPUT_PATH "./dump/lexer_output.json"
#define DEBUG_PARSER_OUTPUT_PATH "./dump/parser_output.json"

int main(int argc, char* argv[]) {
    // Reading code
    if(argc != 2)
        return 1;

    std::ifstream file(argv[1]);
    if(!file)
        return 1;

    std::string file_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    // Lexer
    if(DEBUG_LEXER) {
        std::cout << "=========== Lexer Debug ===========" << std::endl;
        Lexer debug_lexer(file_content);
        if(!std::string(DEBUG_LEXER_OUTPUT_PATH).empty()) {
            std::ofstream file_c(DEBUG_LEXER_OUTPUT_PATH,
                                 std::ios::trunc); // Open file_c in truncate mode
            if(file_c.is_open()) {
                file_c << ""; // Clear file_c
                file_c.close();
            } else {
                std::cout << "Unable to open file_c";
            }
        }
        while(debug_lexer.current_char != "") {
            std::shared_ptr<token::Token> token = debug_lexer.nextToken();
            if(!std::string(DEBUG_LEXER_OUTPUT_PATH).empty()) {
                std::ofstream file(DEBUG_LEXER_OUTPUT_PATH,
                                   std::ios::app); // Open file in append mode
                if(file.is_open()) {
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
    Lexer l(file_content);
    parser::Parser p(std::make_shared<Lexer>(l));
    if(DEBUG_PARSER) {
        std::cout << "=========== Parser Debug ===========" << std::endl;
        auto program = p.parseProgram();
        if(!std::string(DEBUG_PARSER_OUTPUT_PATH).empty()) {
            std::ofstream file(DEBUG_PARSER_OUTPUT_PATH,
                               std::ios::trunc); // Open file in append mode
            if(file.is_open()) {
                file << program->toJSON()->dump(4) << std::endl;
                file.close();
            } else {
                std::cout << "Unable to open file";
            }
        } else {
            std::cout << program->toJSON()->dump(4, ' ', true, nlohmann::json::error_handler_t::replace);
        }
        for(auto& err : p.errors) {
            err->raise(false);
        }
        if(p.errors.size() > 0) {
            return 1;
        }
    }
    return 0;
}