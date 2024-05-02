#include "include/errors.hpp"

void raiseLexerError(std::string message, std::string source, int line_no, int col_no, std::string literal)
{
    std::cerr << "Lexer Error at Line " << line_no << ", Column " << col_no << ": " << message << "\n";

    // Highlight the error in the source line
    std::string line = getStringOnLineNumber(source, line_no);
    size_t pos = line.find(literal);
    if (pos != std::string::npos)
    {
        line.replace(pos, literal.length(), "\033[1;31m" + literal + "\033[0m");
    }

    // Output source line with error highlighted
    std::cerr << "Source Line: " << line << "\n";

    std::cerr << std::string(13 + pos, ' ') << std::string(literal.length(), '~') << std::endl;
    exit(1);
}