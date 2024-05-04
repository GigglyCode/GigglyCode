#include <sstream>
#include <vector>
#include "errors.hpp"

void raiseSyntaxError(std::string source, std::string message, int line_no, int col_no, std::string literal, std::string errorType, std::string suggestedFix)
{
    std::cerr << "\n\n\033[1;35m" << std::string(60, '=') << "\033[0m\n\n";

    // Print error message in red color
    std::cerr << "\033[1;31m"
              << "SyntaxError." << errorType << ": \033[0m"
              << "\033[1;97m" << message << "\033[0m\n";

    if (line_no != -1)
    {
        std::cerr << "\033[1;36mSource Context:\033[0m\n";

        if (line_no - 1 > 0)
        {
            std::cerr << "\033[0;32m" << line_no - 1 << " | \033[0m" << getStringOnLineNumber(source, line_no - 1) << "\n";
        }
        std::istringstream iss(literal);
        std::vector<std::string> lines;
        int lit_line_no = line_no;
        std::stringstream concatenated_lines;
        std::string line;
        while (std::getline(iss, line))
        {
            concatenated_lines << (lit_line_no == line_no ? "\033[1;31m\033[4m" : "\033[0;32m" + std::to_string(lit_line_no) + " | \033[1;31m\033[4m") << line << "\033[0m\n";
            lit_line_no++;
        }
        literal = concatenated_lines.str();
        line = (getStringOnLineNumber(source, line_no).substr(0, col_no - 1) + literal + "\033[0m");
        std::cerr << "\033[0;32m" << line_no << " | \033[0m" << line << "\n";
        if (line_no + 1 <= getNumberOfLines(source) && lit_line_no == line_no)
        {
            std::cerr << "\033[0;32m" << line_no + 1 << " | \033[0m: " << getStringOnLineNumber(source, line_no + 1) << "\n";
        }
    }
    //  Print suggested fix if it's not empty
    if (!suggestedFix.empty())
    {
        std::cerr << "\033[1;33m"
                  << "Suggested fix: " << suggestedFix << "\033[0m\n\n";
    }
    exit(EXIT_FAILURE);
}