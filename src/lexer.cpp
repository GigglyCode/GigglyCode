#include "include/lexer.hpp"

std::string Token::toString()
{
    return "Token{type: " + tokenTypeString(type) + ", literal: \"" + literal + "\", line_no: " + std::to_string(line_no) + ", col_no: " + std::to_string(col_no) + "}";
}

std::string tokenTypeString(TokenType type)
{
    switch (type)
    {
    case INT:
        return "INT";
    case FLOAT:
        return "FLOAT";
    case ADD:
        return "ADD";
    case SUB:
        return "SUB";
    case MUL:
        return "MUL";
    case DIV:
        return "DIV";
    case MOD:
        return "MOD";
    case POW:
        return "POW";
    case LPAREN:
        return "LPAREN";
    case RPAREN:
        return "RPAREN";
    case SEMICOLON:
        return "SEMICOLON";
    case ILLEGAL:
        return "ILLEGAL";
    case END:
        return "END";
    default:
        return "";
    }
}

void Token::print()
{
    std::cout << toString() << std::endl;
}

Lexer::Lexer(std::string source)
{
    this->source = source;

    this->pos = -1;
    this->readPos = 0;
    this->lineNo = 1;
    this->currentChar = "\0";
    this->_readChar();
}

void Lexer::_readChar()
{
    if (this->readPos >= this->source.length())
    {
        this->currentChar = "\0";
    }
    else
    {
        this->currentChar = this->source[this->readPos];
    }

    this->pos = this->readPos;
    this->readPos++;
};

Token *Lexer::nextToken()
{
    Token *token;

    this->_skipWhitespace();

    if (this->currentChar == "+")
    {
        token = this->_newToken(ADD, this->currentChar);
    }
    else if (this->currentChar == "-")
    {
        token = this->_newToken(SUB, this->currentChar);
    }
    else if (this->currentChar == "*")
    {
        token = this->_newToken(MUL, this->currentChar);
    }
    else if (this->currentChar == "/")
    {
        token = this->_newToken(DIV, this->currentChar);
    }
    else if (this->currentChar == "%")
    {
        token = this->_newToken(MOD, this->currentChar);
    }
    else if (this->currentChar == "^")
    {
        token = this->_newToken(POW, this->currentChar);
    }
    else if (this->currentChar == "(")
    {
        token = this->_newToken(LPAREN, this->currentChar);
    }
    else if (this->currentChar == ")")
    {
        token = this->_newToken(RPAREN, this->currentChar);
    }
    else if (this->currentChar == ";")
    {
        token = this->_newToken(SEMICOLON, this->currentChar);
    }
    else if (this->currentChar == "\0")
    {
        token = this->_newToken(END, "");
    }
    else
    {
        if (this->_isDigit(this->currentChar))
        {
            token = this->_readNumber();
            return token;
        }
        else
        {
            token = this->_newToken(ILLEGAL, this->currentChar);
        }
    }
    this->_readChar();
    return token;
}

Token *Lexer::_newToken(TokenType type, std::string currentChar)
{
    return new Token(type, currentChar, this->lineNo, this->pos);
}

Token *Lexer::_readNumber()
{
    int st = this->pos;
    int dot_count = 0;
    std::string number = "";
    while (this->_isDigit(this->currentChar) || this->currentChar == ".")
    {
        if (this->currentChar == ".")
        {
            dot_count++;
            if (dot_count > 1)
            {
                printf("Invalid number at line %d, column %d\n", this->lineNo, this->pos);
                return this->_newToken(ILLEGAL, this->currentChar);
            }
        }
        number += this->currentChar;
        this->_readChar();
        if (this->currentChar == "\0")
            break;
    }
    if (dot_count == 0)
    {
        return this->_newToken(INT, number);
    }
    return this->_newToken(FLOAT, number);
};

void Lexer::_skipWhitespace()
{
    while (this->currentChar == " " || this->currentChar == "\t" || this->currentChar == "\n" || this->currentChar == "\r")
    {
        if (this->currentChar == "\n")
        {
            this->lineNo++;
        }

        this->_readChar();
    }
}

bool Lexer::_isDigit(std::string character)
{
    return character >= "0" && character <= "9";
};