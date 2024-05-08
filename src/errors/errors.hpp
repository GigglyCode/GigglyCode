#include <iostream>
#include "../lexer/tokens.hpp"
#include "../lexer/lexer.hpp"

namespace errors
{
    class Error
    {
    public:
        std::string source;
        int st_line;
        int end_line;
        std::string message;
        std::string suggestedFix;
        Error(std::string source, int st_line, int end_line, std::string message, std::string suggestedFix = "") : source(source), st_line(st_line), end_line(end_line), message(message), suggestedFix(suggestedFix) {}
        Error(){};
        virtual void raise(bool terminate = true);
    };

    class SyntaxError : public Error
    {
    public:
        token::Token token;
        SyntaxError(std::string source, token::Token token, std::string message = "", std::string suggestedFix = "") : Error(source, -1, -1, message, suggestedFix), token(token) {}
        void raise(bool terminate = true);
    };

    void raiseSyntaxError(std::string source, token::Token token, std::string message = "", std::string suggestedFix = "");

    class NoPrefixParseFnError : public Error
    {
    public:
        token::Token token;
        NoPrefixParseFnError(std::string source, token::Token token, std::string message = "", std::string suggestedFix = "") : Error(source, -1, -1, message, suggestedFix), token(token) {}
        void raise(bool terminate = true);
    };
}