#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <memory>

enum class TokenType
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

std::string tokenTypeString(TokenType type);

class Token
{
public:
    TokenType type;
    std::string literal;
    int line_no;
    int col_no;

    Token(TokenType type, std::string literal, int line_no, int col_no)
        : type(type), literal(literal), line_no(line_no), col_no(col_no){};

    Token(TokenType type, int line_no, int col_no)
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
    int readPos;
    int lineNo;
    std::string currentChar;
    Lexer(std::string source);
    std::shared_ptr<Token> nextToken();

private:
    void _readChar();
    void _skipWhitespace();
    std::shared_ptr<Token> _newToken(TokenType type, std::string currentChar);
    std::shared_ptr<Token> _readNumber();
    bool _isDigit(std::string character);
};