#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#pragma once

enum TokenType
{
    INT,
    FLOAT,
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    POW,
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
    Token *nextToken();

private:
    void _readChar();
    void _skipWhitespace();
    Token *_newToken(TokenType type, std::string currentChar);
    Token *_readNumber();
    bool _isDigit(std::string character);
};