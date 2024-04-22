#include "include/lexer.hpp"

std::string Token::toString()
{
    return "Token{type: " + tokenTypeString(type) + ", literal: \"" + literal + "\", line_no: " + std::to_string(line_no) + ", col_no: " + std::to_string(col_no) + "}";
}

Token::Token(){};

std::string tokenTypeString(TokenType type)
{
    switch (type)
    {
    case TokenType::INT:
        return "INT";
    case TokenType::FLOAT:
        return "FLOAT";
    case TokenType::PLUS:
        return "PLUS";
    case TokenType::MINUS:
        return "MINUS";
    case TokenType::ASTERISK:
        return "ASTERISK";
    case TokenType::FSLASH:
        return "FSLASH";
    case TokenType::PERCENT:
        return "PERCENT";
    case TokenType::CARET:
        return "CARET";
    case TokenType::LPAREN:
        return "LPAREN";
    case TokenType::RPAREN:
        return "RPAREN";
    case TokenType::SEMICOLON:
        return "SEMICOLON";
    case TokenType::ILLEGAL:
        return "ILLEGAL";
    case TokenType::END:
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

std::shared_ptr<Token> Lexer::nextToken()
{
    std::shared_ptr<Token> token;

    this->_skipWhitespace();

    if (this->currentChar == "+")
    {
        token = this->_newToken(PLUS, this->currentChar);
    }
    else if (this->currentChar == "-")
    {
        token = this->_newToken(TokenType::MINUS, this->currentChar);
    }
    else if (this->currentChar == "*")
    {
        token = this->_newToken(TokenType::ASTERISK, this->currentChar);
    }
    else if (this->currentChar == "/")
    {
        token = this->_newToken(TokenType::FSLASH, this->currentChar);
    }
    else if (this->currentChar == "%")
    {
        token = this->_newToken(TokenType::PERCENT, this->currentChar);
    }
    else if (this->currentChar == "^")
    {
        token = this->_newToken(TokenType::CARET, this->currentChar);
    }
    else if (this->currentChar == "(")
    {
        token = this->_newToken(TokenType::LPAREN, this->currentChar);
    }
    else if (this->currentChar == ")")
    {
        token = this->_newToken(TokenType::RPAREN, this->currentChar);
    }
    else if (this->currentChar == ";")
    {
        token = this->_newToken(TokenType::SEMICOLON, this->currentChar);
    }
    else if (this->currentChar == "\0")
    {
        token = this->_newToken(TokenType::END, "");
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
            token = this->_newToken(TokenType::ILLEGAL, this->currentChar);
        }
    }
    this->_readChar();
    return token;
}

std::shared_ptr<Token> Lexer::_newToken(TokenType type, std::string currentChar)
{
    return std::make_shared<Token>(type, currentChar, this->lineNo, this->pos);
}

std::shared_ptr<Token> Lexer::_readNumber()
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
                return this->_newToken(TokenType::ILLEGAL, this->currentChar);
            }
        }
        number += this->currentChar;
        this->_readChar();
        if (this->currentChar == "\0")
            break;
    }
    if (dot_count == 0)
    {
        return this->_newToken(TokenType::INT, number);
    }
    return this->_newToken(TokenType::FLOAT, number);
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