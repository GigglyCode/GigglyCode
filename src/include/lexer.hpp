#ifndef LEXER_HPP
#define LEXER_HPP
#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <memory>

enum class tokenType
{
    // Special Tokens
    ILLEGAL,
    END,

    // Data Types
    IDENT,
    INT,
    FLOAT,

    // Arthematic Operator
    PLUS,
    MINUS,
    ASTERISK,
    PERCENT,
    CARET,
    FSLASH,
    BSLASH,

    // Symbols
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    COLON,
    SEMICOLON,
    RARROW,
    EQUALS,

    // Keywords
    LET,
    DEF,
    RETURN,
};

tokenType lookupIdent(std::string ident);

std::string tokenTypeString(tokenType type);

class Token
{
public:
    tokenType type;
    std::string literal;
    int line_no;
    int col_no;

    Token(){};
    Token(tokenType type, int line_no, int col_no)
        : type(type), line_no(line_no), col_no(col_no){};
    Token(tokenType type, std::string literal, int line_no, int col_no)
        : type(type), literal(literal), line_no(line_no), col_no(col_no){};
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
    std::shared_ptr<std::string> _peekChar();
    void _skipWhitespace();
    std::shared_ptr<Token> _newToken(tokenType type, std::string currentChar);
    std::shared_ptr<Token> _readNumber();
    bool _isDigit(std::string character);
    bool _isLetter(std::string character);
    std::shared_ptr<std::string> _readIdentifier();
};
#endif
