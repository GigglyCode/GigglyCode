#include "include/lexer.hpp"

Lexer::Lexer(std::string source)
{
    this->source = source;

    this->pos = -1;
    this->readPos = 0;
    this->lineNo = 1;
    this->colNo = -1;
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
    this->colNo++;
};

std::shared_ptr<std::string> Lexer::_peekChar()
{
    if (this->readPos >= this->source.length())
    {
        return std::make_shared<std::string>("\0");
    }
    else
    {
        return std::make_shared<std::string>(1, this->source[this->readPos]);
    }
}

token::tokenType Lexer::_lookupIdent(std::shared_ptr<std::string> ident)
{
    if (*ident == "var")
    {
        return token::tokenType::Var;
    }
    else if (*ident == "def")
    {
        return token::tokenType::Def;
    }
    else if (*ident == "return")
    {
        return token::tokenType::Return;
    }
    return token::tokenType::Identifier;
};

std::shared_ptr<token::Token> Lexer::nextToken()
{
    std::shared_ptr<token::Token> token;

    this->_skipWhitespace();

    if (this->currentChar == "+")
    {
        if (*this->_peekChar() == "+")
        {
            token = this->_newToken(token::tokenType::Increment, this->currentChar + *this->_peekChar());
            this->_readChar();
        }
        else if (*this->_peekChar() == "=")
        {
            token = this->_newToken(token::tokenType::PlusEqual, this->currentChar + *this->_peekChar());
            this->_readChar();
        }
        else
            token = this->_newToken(token::tokenType::Plus, this->currentChar);
    }
    else if (this->currentChar == "-")
    {
        if (*this->_peekChar() == ">")
        {
            token = this->_newToken(token::tokenType::RightArrow, this->currentChar + *this->_peekChar());
            this->_readChar();
        }
        else if (*this->_peekChar() == "-")
        {
            token = this->_newToken(token::tokenType::Decrement, this->currentChar + *this->_peekChar());
            this->_readChar();
        }
        else if (this->_isDigit(*(this->_peekChar())))
        {
            this->_readChar();
            token = this->_readNumber();
            token->literal = "-" + token->literal;
        }
        else if (*this->_peekChar() == "=")
        {
            token = this->_newToken(token::tokenType::DashEqual, this->currentChar + *this->_peekChar());
            this->_readChar();
        }
        else
            token = this->_newToken(token::tokenType::Dash, this->currentChar);
    }
    else if (this->currentChar == "*")
    {
        if (*this->_peekChar() == "=")
        {
            token = this->_newToken(token::tokenType::AsteriskEqual, this->currentChar + *this->_peekChar());
            this->_readChar();
        }
        else
            token = this->_newToken(token::tokenType::Asterisk, this->currentChar);
    }
    else if (this->currentChar == "/")
    {
        if (*this->_peekChar() == "=")
        {
            token = this->_newToken(token::tokenType::ForwardSlashEqual, this->currentChar + *this->_peekChar());
            this->_readChar();
        }
        else
            token = this->_newToken(token::tokenType::ForwardSlash, this->currentChar);
    }
    else if (this->currentChar == "%")
    {
        if (*this->_peekChar() == "=")
        {
            token = this->_newToken(token::tokenType::PercentEqual, this->currentChar + *this->_peekChar());
            this->_readChar();
        }
        else
            token = this->_newToken(token::tokenType::Percent, this->currentChar);
    }
    else if (this->currentChar == "^")
    {
        if (*this->_peekChar() == "=")
        {
            token = this->_newToken(token::tokenType::CaretEqual, this->currentChar + *this->_peekChar());
            this->_readChar();
        }
        else
            token = this->_newToken(token::tokenType::Caret, this->currentChar);
    }
    else if (this->currentChar == "=")
    {
        if (*this->_peekChar() == "=")
        {
            token = this->_newToken(token::tokenType::EqualEqual, this->currentChar + *this->_peekChar());
            this->_readChar();
        }
        else
            token = this->_newToken(token::tokenType::Equals, this->currentChar);
    }
    else if (this->currentChar == ">")
    {
        if (*this->_peekChar() == "=")
        {
            token = this->_newToken(token::tokenType::GreaterThanOrEqual, this->currentChar + *this->_peekChar());
            this->_readChar();
        }
        else
            token = this->_newToken(token::tokenType::GreaterThan, this->currentChar);
    }
    else if (this->currentChar == "<")
    {
        if (*this->_peekChar() == "=")
        {
            token = this->_newToken(token::tokenType::LessThanOrEqual, this->currentChar + *this->_peekChar());
            this->_readChar();
        }
        else
            token = this->_newToken(token::tokenType::LessThan, this->currentChar);
    }
    else if (this->currentChar == "!")
    {
        if (*this->_peekChar() == "=")
        {
            token = this->_newToken(token::tokenType::NotEquals, this->currentChar + *this->_peekChar());
            this->_readChar();
        }
        else
            token = this->_newToken(token::tokenType::Illegal, this->currentChar);
    }
    else if (this->currentChar == "{")
    {
        token = this->_newToken(token::tokenType::LeftBrace, this->currentChar);
    }
    else if (this->currentChar == "}")
    {
        token = this->_newToken(token::tokenType::RightBrace, this->currentChar);
    }
    else if (this->currentChar == "(")
    {
        token = this->_newToken(token::tokenType::LeftParen, this->currentChar);
    }
    else if (this->currentChar == ")")
    {
        token = this->_newToken(token::tokenType::RightParen, this->currentChar);
    }
    else if (this->currentChar == "{")
    {
        token = this->_newToken(token::tokenType::LeftBrace, this->currentChar);
    }
    else if (this->currentChar == "}")
    {
        token = this->_newToken(token::tokenType::RightBrace, this->currentChar);
    }
    else if (this->currentChar == ":")
    {
        token = this->_newToken(token::tokenType::Colon, this->currentChar);
    }
    else if (this->currentChar == ";")
    {
        token = this->_newToken(token::tokenType::Semicolon, this->currentChar);
    }
    else if (this->currentChar == "\0")
    {
        token = this->_newToken(token::tokenType::EndOfFile, "");
    }
    else
    {
        if (this->_isLetter(this->currentChar))
        {
            std::shared_ptr<std::string> ident = this->_readIdentifier();
            token = this->_newToken(this->_lookupIdent(ident), *ident);
            return token;
        }
        else if (this->_isDigit(this->currentChar))
        {
            token = this->_readNumber();
            return token;
        }
        else
        {
            token = this->_newToken(token::tokenType::Illegal, this->currentChar);
        }
    }
    this->_readChar();
    return token;
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
        if (this->currentChar == "\0")
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
        while (this->currentChar != "\n" && this->currentChar != "\0")
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