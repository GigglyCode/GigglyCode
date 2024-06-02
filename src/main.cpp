#include "compiler/compiler.hpp"
#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include <fstream>
#include <iostream>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/raw_ostream.h>
#include <string>


#define DEBUG_LEXER false
#define DEBUG_PARSER false
#define DEBUG_COMPILER true
#define DEBUG_LEXER_OUTPUT_PATH "./dump/lexer_output"
#define DEBUG_PARSER_OUTPUT_PATH "./dump/parser_output.json"
#define DEBUG_COMPILER_OUTPUT_PATH "./dump/compiler_output.ll"

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
        if(!std::string(DEBUG_LEXER_OUTPUT_PATH).empty()) {
            std::cout << "Lexer output dumped to " << DEBUG_LEXER_OUTPUT_PATH << std::endl;
        }
    }
    Lexer l(file_content);
    parser::Parser p(std::make_shared<Lexer>(l));
    auto program = p.parseProgram();
    if(DEBUG_PARSER) {
        std::cout << "=========== Parser Debug ===========" << std::endl;
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
        if(!std::string(DEBUG_PARSER_OUTPUT_PATH).empty()) {
            std::cout << "Parser output dumped to " << DEBUG_PARSER_OUTPUT_PATH << std::endl;
        }
    } else {
        for(auto& err : p.errors) {
            err->raise(false);
        }
        if(p.errors.size() > 0) {
            return 1;
        }
    }
    compiler::Compiler c(file_content);
    if(DEBUG_COMPILER) {
        std::cout << "=========== Compiler Debug ===========" << std::endl;
        c.compile(program);
        std::error_code EC;
        llvm::raw_fd_ostream file(DEBUG_COMPILER_OUTPUT_PATH, EC, llvm::sys::fs::OF_None);
        if(EC) {
            std::cerr << "Could not open file: " << EC.message() << std::endl;
            return 1;
        }
        c.llvm_module->print(file, nullptr);
        file.close();
        std::cout << "Compiler output dumped to " << DEBUG_COMPILER_OUTPUT_PATH << std::endl;
    }
    return 0;
}
