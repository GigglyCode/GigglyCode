#include <iostream>
#include "../lexer/tokens.hpp"
#include "../lexer/lexer.hpp"

// void raiseLexerError(std::string message, std::string source, int line_no, int col_no, std::string literal);
void raiseSyntaxError(std::string source, std::string message = "", int line_no = -1, int col_no = -1, std::string literal = "", std::string errorType = "", std::string suggestedFix = "");