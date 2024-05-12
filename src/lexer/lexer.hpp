#ifndef LEXER_HPP
#define LEXER_HPP
#include "tokens.hpp"
#include <memory>
#include <string>

std::string getStringOnLineNumber(const std::string &str, int lineNumber);
int getNumberOfLines(const std::string &str);

class Lexer {
  public:
    std::string source;
    int pos;
    unsigned int lineNo;
    int colNo;
    std::string currentChar;
    explicit Lexer(const std::string &source);
    std::shared_ptr<token::Token> nextToken();

  private:
    token::tokenType _lookupIdent(std::shared_ptr<std::string> ident);
    void _readChar();
    std::shared_ptr<std::string> _peekChar(int offset = 1);
    void _skipWhitespace();
    std::shared_ptr<token::Token> _newToken(token::tokenType type,
                                            std::string currentChar);
    std::shared_ptr<token::Token> _readNumber();
    bool _isDigit(const std::string &character);
    bool _isLetter(const std::string &character);
    std::shared_ptr<std::string> _isString();
    std::shared_ptr<std::string> _readIdentifier();
    std::shared_ptr<std::string> _readString(const std::string &quote);
};
#endif