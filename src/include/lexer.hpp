#ifndef LEXER_HPP
#define LEXER_HPP
#include <string>
#include <memory>
#include "tokens.hpp"

class Lexer
{
public:
    std::string source;
    int pos;
    unsigned int readPos;
    unsigned int lineNo;
    int colNo;
    std::string currentChar;
    Lexer(std::string source);
    std::shared_ptr<token::Token> nextToken();

private:
    token::tokenType _lookupIdent(std::shared_ptr<std::string> ident);
    void _readChar();
    std::shared_ptr<std::string> _peekChar();
    void _skipWhitespace();
    std::shared_ptr<token::Token> _newToken(token::tokenType type, std::string currentChar);
    std::shared_ptr<token::Token> _readNumber();
    bool _isDigit(std::string character);
    bool _isLetter(std::string character);
    std::shared_ptr<std::string> _readIdentifier();
};
#endif