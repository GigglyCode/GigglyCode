#ifndef LEXER_HPP
#define LEXER_HPP
#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <memory>

enum class tokenType
{
    INT,
    FLOAT,
    PLUS,
    MINUS,
    ASTERISK,
    FSLASH,
    BSLASH,
    PERCENT,
    CARET,
    LPAREN,
    RPAREN,
    SEMICOLON,
    ILLEGAL,
    END
};

std::string tokenTypeString(tokenType type);

class Token
{
public:
    tokenType type;
    std::string literal;
    int line_no;
    int col_no;

    Token(tokenType type, std::string literal, int line_no, int col_no)
        : type(type), literal(literal), line_no(line_no), col_no(col_no){};

    Token(tokenType type, int line_no, int col_no)
        : type(type), literal(""), line_no(line_no), col_no(col_no){};
    Token();
    std::string toString();
    void print();
};

class Lexer
{
public:
    std::string source;
    int pos;
    unsigned int readPos;
    unsigned int lineNo;
    std::string currentChar;
    Lexer(std::string source);
    std::shared_ptr<Token> nextToken();

private:
    void _readChar();
    void _skipWhitespace();
    std::shared_ptr<Token> _newToken(tokenType type, std::string currentChar);
    std::shared_ptr<Token> _readNumber();
    bool _isDigit(std::string character);
};
#endif
