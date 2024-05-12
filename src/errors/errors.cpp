#include "errors.hpp"
#include <sstream>
#include <vector>

void errors::Error::raise(bool terminate) {
    std::cerr << "\n\n\033[1;35m" << std::string(60, '=') << "\033[0m\n\n";

    // Print error message in red color
    std::cerr << "\033[1;31m"
              << "Error: \033[0m"
              << "\033[1;97m" << message << "\033[0m\n";

    if(st_line != -1 && end_line != -1) {
        std::cerr << "\033[1;36mSource Context:\033[0m\n";

        if(st_line - 1 > 0) {
            std::cerr << "\033[0;32m" << st_line - 1 << " | \033[0m" << getStringOnLineNumber(source, st_line - 1) << "\n";
        }
        for(int i = st_line; i <= end_line; i++) {
            std::cerr << "\033[0;32m" << i << " | \033[0m" << getStringOnLineNumber(source, i) << "\n";
        }
        if(end_line + 1 <= getNumberOfLines(source)) {
            std::cerr << "\033[0;32m" << end_line + 1 << " | \033[0m" << getStringOnLineNumber(source, end_line + 1) << "\n";
        }
    }

    //  Print suggested fix if it's not empty
    if(!suggestedFix.empty()) {
        std::cerr << "\033[1;33m"
                  << "Suggested fix: " << suggestedFix << "\033[0m\n\n";
    }
    if(terminate)
        exit(EXIT_FAILURE);
}
void errors::NoPrefixParseFnError::raise(bool terminate) {
    std::cerr << "\n\n\033[1;35m" << std::string(60, '=') << "\033[0m\n\n";

    // Print error message in red color
    std::cerr << "\033[1;31m"
              << "NoPrefixParseFnError: \033[0m"
              << "\033[1;97m" << message << "\033[0m\n";

    std::cerr << "\033[1;36mSource Context:\033[0m\n";

    if(token.line_no - 1 >= 0) {
        std::cerr << "\033[0;32m" << token.line_no - 1 << " | \033[0m" << getStringOnLineNumber(source, token.line_no - 1) << "\n";
    }
    int litLineNo = token.line_no;
    std::stringstream concatenatedLines;
    std::string line;
    while(std::getline(std::istringstream(token.literal), line)) {
        concatenatedLines << (litLineNo == token.line_no ? "\033[1;31m\033[4m" : "\033[0;32m" + std::to_string(litLineNo) + " | \033[1;31m\033[4m")
                          << line << "\033[0m\n";
        litLineNo++;
    }
    std::string literal = concatenatedLines.str();
    line = (getStringOnLineNumber(source, token.line_no).substr(0, token.col_no - 1) + literal + "\033[0m");
    std::cerr << "\033[0;32m" << token.line_no << " | \033[0m" << line << "\n";

    if(token.line_no + 1 <= getNumberOfLines(source) && litLineNo == token.line_no) {
        std::cerr << "\033[0;32m" << token.line_no + 1 << " | \033[0m" << getStringOnLineNumber(source, token.line_no + 1) << "\n";
    }

    //  Print suggested fix if it's not empty
    if(!suggestedFix.empty()) {
        std::cerr << "\033[1;33m"
                  << "Suggested fix: " << suggestedFix << "\033[0m\n\n";
    }
    if(terminate)
        exit(EXIT_FAILURE);
}

void errors::SyntaxError::raise(bool terminate) {
    std::cerr << "\n\n\033[1;35m" << std::string(60, '=') << "\033[0m\n\n";

    // Print error message in red color
    std::cerr << "\033[1;31m"
              << "SyntaxError: \033[0m"
              << "\033[1;97m" << message << "\033[0m\n";

    std::cerr << "\033[1;36mSource Context:\033[0m\n";

    if(token.line_no - 1 >= 0) {
        std::cerr << "\033[0;32m" << token.line_no - 1 << " | \033[0m" << getStringOnLineNumber(source, token.line_no - 1) << "\n";
    }
    int litLineNo = token.line_no;
    std::stringstream concatenatedLines;
    std::string line;
    while(std::getline(std::istringstream(token.literal), line)) {
        concatenatedLines << (litLineNo == token.line_no ? "\033[1;31m\033[4m" : "\033[0;32m" + std::to_string(litLineNo) + " | \033[1;31m\033[4m")
                          << line << "\033[0m\n";
        litLineNo++;
    }
    std::string literal = concatenatedLines.str();
    line = (getStringOnLineNumber(source, token.line_no).substr(0, token.col_no - 1) + literal + "\033[0m");
    std::cerr << "\033[0;32m" << token.line_no << " | \033[0m" << line << "\n";

    if(token.line_no + 1 <= getNumberOfLines(source) && litLineNo == token.line_no) {
        std::cerr << "\033[0;32m" << token.line_no + 1 << " | \033[0m" << getStringOnLineNumber(source, token.line_no + 1) << "\n";
    }

    //  Print suggested fix if it's not empty
    if(!suggestedFix.empty()) {
        std::cerr << "\033[1;33m"
                  << "Suggested fix: " << suggestedFix << "\033[0m\n\n";
    }
    if(terminate)
        exit(EXIT_FAILURE);
}

void errors::raiseSyntaxError(const std::string& source, const token::Token& token, const std::string& message, const std::string& suggestedFix) {
    SyntaxError error(source, token, message, suggestedFix);
    error.raise();
}