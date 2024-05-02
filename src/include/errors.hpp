#include <iostream>
#include "tokens.hpp"
#include "lexer.hpp"

void raiseLexerError(std::string message, std::string source, int line_no, int col_no, std::string literal);