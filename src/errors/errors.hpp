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
        Error(const std::string &source, int st_line, int end_line, const std::string &message, const std::string &suggestedFix = "") : source(source), st_line(st_line), end_line(end_line), message(message), suggestedFix(suggestedFix) {}
        Error(){};
        virtual void raise(bool terminate = true);
    };

    class SyntaxError : public Error
    {
    public:
        token::Token token;
        SyntaxError(const std::string &source, const token::Token &token, const std::string &message = "", const std::string &suggestedFix = "") : Error(source, -1, -1, message, suggestedFix), token(token) {}
        void raise(bool terminate = true) override;
    };

    void raiseSyntaxError(std::string source, token::Token token, std::string message = "", std::string suggestedFix = "");

    class NoPrefixParseFnError : public Error
    {
    public:
        token::Token token;
        NoPrefixParseFnError(const std::string &source, const token::Token &token, const std::string &message = "", const std::string &suggestedFix = "") : Error(source, -1, -1, message, suggestedFix), token(token) {}
        void raise(bool terminate = true) override;
    };
}