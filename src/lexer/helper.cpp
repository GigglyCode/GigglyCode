#include <algorithm>
#include "lexer.hpp"

void Lexer::_readChar()
{
    this->pos++;
    if (this->pos >= static_cast<int>(this->source.length()))
    {
        this->currentChar = "";
    }
    else
    {
        this->currentChar = this->source[this->pos];
    }
    this->colNo++;
};

std::shared_ptr<std::string> Lexer::_peekChar(int offset)
{
    if ((this->pos + offset) >= static_cast<int>(this->source.length()))
    {
        return std::make_shared<std::string>("");
    }
    else
    {
        return std::make_shared<std::string>(1, this->source[this->pos + offset]);
    }
}

std::shared_ptr<token::Token> Lexer::_newToken(token::tokenType type, std::string currentChar)
{
    auto x = std::make_shared<token::Token>(type, currentChar, this->lineNo, this->colNo);
    return x;
}

std::shared_ptr<token::Token> Lexer::_readNumber()
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
                printf("Invalid number at line %d, column %d\n", this->lineNo, this->colNo);
                return this->_newToken(token::tokenType::Illegal, this->currentChar);
            }
        }
        number += this->currentChar;
        this->_readChar();
        if (this->currentChar == "")
            break;
    }
    if (dot_count == 0)
    {
        return this->_newToken(token::tokenType::Integer, number);
    }
    return this->_newToken(token::tokenType::Float, number);
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
            this->colNo = 0;
        }

        this->_readChar();
    }
    if (this->currentChar == "#")
    {
        this->_readChar();
        while (this->currentChar != "\n" && this->currentChar != "")
        {
            this->_readChar();
        }
        this->_skipWhitespace();
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

std::string getStringOnLineNumber(const std::string &str, int lineNumber)
{
    size_t startPos = 0;
    for (int i = 1; i < lineNumber; ++i)
    {
        if ((startPos = str.find('\n', startPos)) == std::string::npos)
        {
            return "";
        }
        startPos++;
    }
    size_t endPos = str.find('\n', startPos);
    return str.substr(startPos, (endPos != std::string::npos) ? endPos - startPos : std::string::npos);
}

int getNumberOfLines(const std::string &str)
{
    return std::count(str.begin(), str.end(), '\n') + 1;
}
