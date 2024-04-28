#include <unordered_map>
#include <iomanip>
#include "include/lexer.hpp"

const std::unordered_map<std::string, tokenType> KEYWORDS = {
    {"let", tokenType::LET},
};

tokenType lookupIdent(std::shared_ptr<std::string> ident)
{
    auto it = KEYWORDS.find(*ident);
    if (it != KEYWORDS.end())
    {
        return it->second;
    }
    return tokenType::IDENT;
}

std::string Token::toString()
{
    // Define ANSI escape codes for colors
    static const std::string colorReset = "\x1b[0m";
    static const std::string colorBlue = "\x1b[34m";
    static const std::string colorYellow = "\x1b[33m";
    static const std::string colorGreen = "\x1b[32m";
    static const std::string colorMagenta = "\x1b[35m";

    // Convert variables to strings
    std::string typeString = tokenTypeString(type);
    std::string literalString = "\"" + literal + "\"";
    std::string lineNoString = std::to_string(line_no);
    std::string colNoString = std::to_string(col_no);

    // Apply padding to each field for alignment
    typeString += std::string(10 - typeString.length(), ' ');
    literalString += std::string(5 - literalString.length(), ' ');
    lineNoString += std::string(2 - lineNoString.length(), ' ');
    colNoString += std::string(2 - colNoString.length(), ' ');

    // Construct the formatted string with colors
    return "Token{type: " + colorBlue + typeString + colorReset +
           ", literal: " + colorYellow + literalString + colorReset +
           ", line_no: " + colorGreen + lineNoString + colorReset +
           ", col_no: " + colorMagenta + colNoString + colorReset + "}";
}

std::string tokenTypeString(tokenType type)
{
    switch (type)
    {
    case tokenType::IDENT:
        return "IDENT";
    case tokenType::INT:
        return "INT";
    case tokenType::FLOAT:
        return "FLOAT";
    case tokenType::PLUS:
        return "PLUS";
    case tokenType::MINUS:
        return "MINUS";
    case tokenType::ASTERISK:
        return "ASTERISK";
    case tokenType::PERCENT:
        return "PERCENT";
    case tokenType::CARET:
        return "CARET";
    case tokenType::FSLASH:
        return "FSLASH";
    case tokenType::BSLASH:
        return "BSLASH";
    case tokenType::LPAREN:
        return "LPAREN";
    case tokenType::RPAREN:
        return "RPAREN";
    case tokenType::COLON:
        return "COLON";
    case tokenType::SEMICOLON:
        return "SEMICOLON";
    case tokenType::EQUALS:
        return "EQUALS";
    case tokenType::ILLEGAL:
        return "ILLEGAL";
    case tokenType::END:
        return "END";
    case tokenType::LET:
        return "LET";
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
        token = this->_newToken(tokenType::PLUS, this->currentChar);
    }
    else if (this->currentChar == "-")
    {
        token = this->_newToken(tokenType::MINUS, this->currentChar);
    }
    else if (this->currentChar == "*")
    {
        token = this->_newToken(tokenType::ASTERISK, this->currentChar);
    }
    else if (this->currentChar == "/")
    {
        token = this->_newToken(tokenType::FSLASH, this->currentChar);
    }
    else if (this->currentChar == "%")
    {
        token = this->_newToken(tokenType::PERCENT, this->currentChar);
    }
    else if (this->currentChar == "^")
    {
        token = this->_newToken(tokenType::CARET, this->currentChar);
    }
    else if (this->currentChar == "=")
    {
        token = this->_newToken(tokenType::EQUALS, this->currentChar);
    }
    else if (this->currentChar == "(")
    {
        token = this->_newToken(tokenType::LPAREN, this->currentChar);
    }
    else if (this->currentChar == ")")
    {
        token = this->_newToken(tokenType::RPAREN, this->currentChar);
    }
    else if (this->currentChar == ":")
    {
        token = this->_newToken(tokenType::COLON, this->currentChar);
    }
    else if (this->currentChar == ";")
    {
        token = this->_newToken(tokenType::SEMICOLON, this->currentChar);
    }
    else if (this->currentChar == "\0")
    {
        token = this->_newToken(tokenType::END, "");
    }
    else
    {
        if (this->_isLetter(this->currentChar))
        {
            std::shared_ptr<std::string> ident = this->_readIdentifier();
            token = this->_newToken(lookupIdent(ident), *ident);
            return token;
        }
        else if (this->_isDigit(this->currentChar))
        {
            token = this->_readNumber();
            return token;
        }
        else
        {
            token = this->_newToken(tokenType::ILLEGAL, this->currentChar);
        }
    }
    this->_readChar();
    return token;
}

std::shared_ptr<Token> Lexer::_newToken(tokenType type, std::string currentChar)
{
    return std::make_shared<Token>(type, currentChar, this->lineNo, this->pos);
}

std::shared_ptr<Token> Lexer::_readNumber()
{
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
                return this->_newToken(tokenType::ILLEGAL, this->currentChar);
            }
        }
        number += this->currentChar;
        this->_readChar();
        if (this->currentChar == "\0")
            break;
    }
    if (dot_count == 0)
    {
        return this->_newToken(tokenType::INT, number);
    }
    return this->_newToken(tokenType::FLOAT, number);
};

std::shared_ptr<std::string> Lexer::_readIdentifier()
{
    std::string identifier = "";
    while (this->_isLetter(this->currentChar) || this->_isDigit(this->currentChar))
    {
        identifier += this->currentChar;
        this->_readChar();
    };
    return std::make_shared<std::string>(identifier);
}

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

bool Lexer::_isLetter(std::string character)
{
    return (character >= "a" && character <= "z") || (character >= "A" && character <= "Z") || character == "_";
};