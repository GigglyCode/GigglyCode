#include <algorithm>

#include "lexer.hpp"
#include "../errors/errors.hpp"

// ---- constructor ----

Lexer::Lexer(const std::string &source)
{
    this->source = source;
    this->pos = -1;
    this->lineNo = 1;
    this->colNo = -1;
    this->currentChar = "";
    this->_readChar();
}

// ---- main lexer ----

std::shared_ptr<token::Token> Lexer::nextToken()
{
    std::shared_ptr<token::Token> token;

    this->_skipWhitespace();

    switch (this->currentChar[0])
    {
        case '+':
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
            {
                token = this->_newToken(token::tokenType::Plus, this->currentChar);
            }
            break;
        case '.':
            if (*this->_peekChar() == "." && *this->_peekChar(2) == ".")
            {
                token = this->_newToken(token::tokenType::Ellipsis, this->currentChar + *this->_peekChar() + *(this->_peekChar(2)));
                this->_readChar();
                this->_readChar();
            }
            else
            {
                token = this->_newToken(token::tokenType::Dot, this->currentChar);
            }
            break;
        case '-':
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
            {
                token = this->_newToken(token::tokenType::Dash, this->currentChar);
            }
            break;
        case '*':
            if (*this->_peekChar() == "=")
            {
                token = this->_newToken(token::tokenType::AsteriskEqual, this->currentChar + *this->_peekChar());
                this->_readChar();
            }
            else if (*this->_peekChar() == "*")
            {
                token = this->_newToken(token::tokenType::AsteriskAsterisk, this->currentChar + *this->_peekChar());
                this->_readChar();
            }
            else
            {
                token = this->_newToken(token::tokenType::Asterisk, this->currentChar);
            }
            break;
        case '/':
            if (*this->_peekChar() == "=")
            {
                token = this->_newToken(token::tokenType::ForwardSlashEqual, this->currentChar + *this->_peekChar());
                this->_readChar();
            }
            else
            {
                token = this->_newToken(token::tokenType::ForwardSlash, this->currentChar);
            }
            break;
        case '%':
            if (*this->_peekChar() == "=")
            {
                token = this->_newToken(token::tokenType::PercentEqual, this->currentChar + *this->_peekChar());
                this->_readChar();
            }
            else
            {
                token = this->_newToken(token::tokenType::Percent, this->currentChar);
            }
            break;
        case '^':
            if (*this->_peekChar() == "=")
            {
                token = this->_newToken(token::tokenType::CaretEqual, this->currentChar + *this->_peekChar());
                this->_readChar();
            }
            else
            {
                token = this->_newToken(token::tokenType::BitwiseXor, this->currentChar);
            }
            break;
        case '=':
            if (*this->_peekChar() == "=")
            {
                token = this->_newToken(token::tokenType::EqualEqual, this->currentChar + *this->_peekChar());
                this->_readChar();
            }
            else
            {
                token = this->_newToken(token::tokenType::Equals, this->currentChar);
            }
            break;
        case '>':
            if (*this->_peekChar() == "=")
            {
                token = this->_newToken(token::tokenType::GreaterThanOrEqual, this->currentChar + *this->_peekChar());
                this->_readChar();
            }
            else if (*this->_peekChar() == ">")
            {
                token = this->_newToken(token::tokenType::RightShift, this->currentChar + *this->_peekChar());
                this->_readChar();
            }
            else
            {
                token = this->_newToken(token::tokenType::GreaterThan, this->currentChar);
            }
            break;
        case '<':
            if (*this->_peekChar() == "=")
            {
                token = this->_newToken(token::tokenType::LessThanOrEqual, this->currentChar + *this->_peekChar());
                this->_readChar();
            }
            else if (*this->_peekChar() == "<")
            {
                token = this->_newToken(token::tokenType::LeftShift, this->currentChar + *this->_peekChar());
                this->_readChar();
            }
            else
            {
                token = this->_newToken(token::tokenType::LessThan, this->currentChar);
            }
            break;
        case '!':
            if (*this->_peekChar() == "=")
            {
                token = this->_newToken(token::tokenType::NotEquals, this->currentChar + *this->_peekChar());
                this->_readChar();
            }
            else
            {
                token = this->_newToken(token::tokenType::Illegal, this->currentChar);
            }
            break;
        case '{':
            token = this->_newToken(token::tokenType::LeftBrace, this->currentChar);
            break;
        case '}':
            token = this->_newToken(token::tokenType::RightBrace, this->currentChar);
            break;
        case '(':
            token = this->_newToken(token::tokenType::LeftParen, this->currentChar);
            break;
        case ')':
            token = this->_newToken(token::tokenType::RightParen, this->currentChar);
            break;
        case '[':
            token = this->_newToken(token::tokenType::LeftBracket, this->currentChar);
            break;
        case ']':
            token = this->_newToken(token::tokenType::RightBracket, this->currentChar);
            break;
        case ':':
            token = this->_newToken(token::tokenType::Colon, this->currentChar);
            break;
        case ';':
            token = this->_newToken(token::tokenType::Semicolon, this->currentChar);
            break;
        case '&':
            if (*this->_peekChar() == "&")
            {
                token = this->_newToken(token::tokenType::BitwiseAnd, this->currentChar + *this->_peekChar());
                this->_readChar();
            }
            else
            {
                token = this->_newToken(token::tokenType::Illegal, this->currentChar);
            }
            break;
        case '|':
            if (*this->_peekChar() == "|")
            {
                token = this->_newToken(token::tokenType::BitwiseOr, this->currentChar + *this->_peekChar());
                this->_readChar();
            }
            else
            {
                token = this->_newToken(token::tokenType::Illegal, this->currentChar);
            }
            break;
        case '~':
            token = this->_newToken(token::tokenType::BitwiseNot, this->currentChar);
            break;
        case ',':
            token = this->_newToken(token::tokenType::Comma, this->currentChar);
            break;
        default:
            if (this->currentChar == "")
            {
                token = this->_newToken(token::tokenType::EndOfFile, "");
            }
            else if (*this->_isString() != "")
            {
                std::shared_ptr<std::string> str = this->_readString(*this->_isString());
                token = this->_newToken(token::tokenType::String, *str);
                return token;
            }
            else if (this->_isLetter(this->currentChar))
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

// ---- identifier parser ----

token::tokenType Lexer::_lookupIdent(std::shared_ptr<std::string> ident)
{
    if (*ident == "and")
    {
        return token::tokenType::And;
    }
    else if (*ident == "or")
    {
        return token::tokenType::Or;
    }
    else if (*ident == "not")
    {
        return token::tokenType::Not;
    }
    else if (*ident == "def")
    {
        return token::tokenType::Def;
    }
    else if (*ident == "return")
    {
        return token::tokenType::Return;
    }
    else if (*ident == "if")
    {
        return token::tokenType::If;
    }
    else if (*ident == "else")
    {
        return token::tokenType::Else;
    }
    else if (*ident == "elif")
    {
        return token::tokenType::ElIf;
    }
    else if (*ident == "is")
    {
        return token::tokenType::Is;
    }
    else if (*ident == "while")
    {
        return token::tokenType::While;
    }
    else if (*ident == "for")
    {
        return token::tokenType::For;
    }
    else if (*ident == "in")
    {
        return token::tokenType::In;
    }
    else if (*ident == "break")
    {
        return token::tokenType::Break;
    }
    else if (*ident == "continue")
    {
        return token::tokenType::Continue;
    }
    else if (*ident == "True")
    {
        return token::tokenType::True;
    }
    else if (*ident == "False")
    {
        return token::tokenType::False;
    }
    else if (*ident == "MayBe")
    {
        return token::tokenType::Maybe;
    }
    else if (*ident == "None")
    {
        return token::tokenType::None;
    }
    return token::tokenType::Identifier;
}

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

bool Lexer::_isLetter(const std::string &character)
{
    return (character >= "a" && character <= "z") || (character >= "A" && character <= "Z") || character == "_";
}

// ---- string parser ----

std::shared_ptr<std::string> Lexer::_readString(const std::string &quote)
{
    std::string str = "";
    std::string literal = quote;
    int stColNo = this->colNo;
    if (quote == "\"\"\"" || quote == "'''")
    {
        this->_readChar();
        this->_readChar();
    }
    while (true)
    {
        this->_readChar();
        if (this->currentChar == "")
        {
            errors::raiseSyntaxError(this->source, token::Token(token::tokenType::String, literal, this->lineNo, stColNo), "Unterminated string literal", "Add a closing " + quote + " to terminate the string literal");
        }
        else if (this->currentChar == "\n" && quote != "\"\"\"" && quote != "'''")
        {
            errors::raiseSyntaxError(this->source, token::Token(token::tokenType::String, literal, this->lineNo, stColNo), "Unterminated string literal", "Add a closing " + quote + " to terminate the string literal");
        }
        else if (this->currentChar == "\\")
        {
            this->_readChar();
            if (this->currentChar == "\"")
            {
                str += "\"";
                literal += "\\\"";
            }
            else if (this->currentChar == "'")
            {
                str += "'";
                literal += "\\'";
            }
            else if (this->currentChar == "n")
            {
                str += "\n";
                literal += "\\n";
            }
            else if (this->currentChar == "t")
            {
                str += "\t";
                literal += "\\t";
            }
            else if (this->currentChar == "r")
            {
                str += "\r";
                literal += "\\r";
            }
            else if (this->currentChar == "b")
            {
                str += "\b";
                literal += "\\b";
            }
            else if (this->currentChar == "f")
            {
                str += "\f";
                literal += "\\f";
            }
            else if (this->currentChar == "v")
            {
                str += "\v";
                literal += "\\v";
            }
            else if (this->currentChar == "\\")
            {
                str += "\\";
                literal += "\\\\";
            }
            else
            {
                str += "\\" + this->currentChar;
                literal += "\\" + this->currentChar;
            }
        }
        else if ((this->currentChar == quote))
        {
            this->_readChar();
            break;
        }
        else if (this->currentChar + *this->_peekChar() + *this->_peekChar(2) == quote)
        {
            this->_readChar();
            this->_readChar();
            this->_readChar();
            break;
        }
        else
        {
            str += this->currentChar;
            literal += this->currentChar;
        }
    }
    return std::make_shared<std::string>(str);
}

std::shared_ptr<std::string> Lexer::_isString()
{
    if (this->currentChar == "\"")
    {
        if (*this->_peekChar(1) == "\"")
        {
            if (*this->_peekChar(2) == "\"")
            {
                return std::make_shared<std::string>("\"\"\"");
            }
            else
            {
                return std::make_shared<std::string>("\"");
            }
        }
        else
        {
            return std::make_shared<std::string>("\"");
        }
    }
    else if (this->currentChar == "'")
    {
        if (*this->_peekChar(1) == "'")
        {
            if (*this->_peekChar(2) == "'")
            {
                return std::make_shared<std::string>("'''");
            }
            else
            {
                return std::make_shared<std::string>("'");
            }
        }
        else
        {
            return std::make_shared<std::string>("'");
        }
    }
    return std::make_shared<std::string>("");
}

// ---- number parser ----

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
                printf("Invalid number at line %u, column %i\n", this->lineNo, this->colNo);
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
}

bool Lexer::_isDigit(const std::string &character)
{
    return character >= "0" && character <= "9";
}

// ---- misc ----

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

// ---- helpers ----

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
}

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
