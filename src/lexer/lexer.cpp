#include "lexer.hpp"
#include "../errors/errors.hpp"

Lexer::Lexer(const std::string &source)
{
    this->source = source;
    this->pos = -1;
    this->lineNo = 1;
    this->colNo = -1;
    this->currentChar = "";
    this->_readChar();
}

token::tokenType Lexer::_lookupIdent(std::shared_ptr<std::string> ident)
{
    if (*ident == "and")
        return token::tokenType::And;
    else if (*ident == "or")
        return token::tokenType::Or;
    else if (*ident == "not")
        return token::tokenType::Not;
    else if (*ident == "def")
        return token::tokenType::Def;
    else if (*ident == "return")
        return token::tokenType::Return;
    else if (*ident == "if")
        return token::tokenType::If;
    else if (*ident == "else")
        return token::tokenType::Else;
    else if (*ident == "elif")
        return token::tokenType::ElIf;
    else if (*ident == "is")
        return token::tokenType::Is;
    else if (*ident == "while")
        return token::tokenType::While;
    else if (*ident == "for")
        return token::tokenType::For;
    else if (*ident == "in")
        return token::tokenType::In;
    else if (*ident == "break")
        return token::tokenType::Break;
    else if (*ident == "continue")
        return token::tokenType::Continue;
    else if (*ident == "True")
        return token::tokenType::True;
    else if (*ident == "False")
        return token::tokenType::False;
    else if (*ident == "MayBe")
        return token::tokenType::Maybe;
    else if (*ident == "None")
        return token::tokenType::None;
    else
        return token::tokenType::Identifier;
}

std::shared_ptr<token::Token> Lexer::nextToken()
{
    std::shared_ptr<token::Token> token;

    this->_skipWhitespace();

    if (this->currentChar == "+") {
        if (*this->_peekChar() == "+") {
            token = this->_newToken(token::tokenType::Increment, this->currentChar + *this->_peekChar());
            this->_readChar();
        } else if (*this->_peekChar() == "=") {
            token = this->_newToken(token::tokenType::PlusEqual, this->currentChar + *this->_peekChar());
            this->_readChar();
        } else {
            token = this->_newToken(token::tokenType::Plus, this->currentChar);
        }
    } else if (this->currentChar == ".") {
        if (*this->_peekChar() == "." && *this->_peekChar(2) == ".") {
            token = this->_newToken(token::tokenType::Ellipsis, this->currentChar + *this->_peekChar() + *(this->_peekChar(2)));
            this->_readChar();
            this->_readChar();
        } else {
            token = this->_newToken(token::tokenType::Dot, this->currentChar);
        }
    } else if (this->currentChar == "-") {
        if (*this->_peekChar() == ">") {
            token = this->_newToken(token::tokenType::RightArrow, this->currentChar + *this->_peekChar());
            this->_readChar();
        } else if (*this->_peekChar() == "-") {
            token = this->_newToken(token::tokenType::Decrement, this->currentChar + *this->_peekChar());
            this->_readChar();
        } else if (this->_isDigit(*(this->_peekChar()))) {
            this->_readChar();
            token = this->_readNumber();
            token->literal = "-" + token->literal;
        } else if (*this->_peekChar() == "=") {
            token = this->_newToken(token::tokenType::DashEqual, this->currentChar + *this->_peekChar());
            this->_readChar();
        } else {
            token = this->_newToken(token::tokenType::Dash, this->currentChar);
        }
    } else if (this->currentChar == "*") {
        if (*this->_peekChar() == "=") {
            token = this->_newToken(token::tokenType::AsteriskEqual, this->currentChar + *this->_peekChar());
            this->_readChar();
        } else if (*this->_peekChar() == "*") {
            token = this->_newToken(token::tokenType::AsteriskAsterisk, this->currentChar + *this->_peekChar());
            this->_readChar();
        } else {
            token = this->_newToken(token::tokenType::Asterisk, this->currentChar);
        }
    } else if (this->currentChar == "/") {
        if (*this->_peekChar() == "=") {
            token = this->_newToken(token::tokenType::ForwardSlashEqual, this->currentChar + *this->_peekChar());
            this->_readChar();
        } else {
            token = this->_newToken(token::tokenType::ForwardSlash, this->currentChar);
        }
    } else if (this->currentChar == "%") {
        if (*this->_peekChar() == "=") {
            token = this->_newToken(token::tokenType::PercentEqual, this->currentChar + *this->_peekChar());
            this->_readChar();
        } else {
            token = this->_newToken(token::tokenType::Percent, this->currentChar);
        }
    } else if (this->currentChar == "^") {
        if (*this->_peekChar() == "=") {
            token = this->_newToken(token::tokenType::CaretEqual, this->currentChar + *this->_peekChar());
            this->_readChar();
        } else {
            token = this->_newToken(token::tokenType::BitwiseXor, this->currentChar);
        }
    } else if (this->currentChar == "=") {
        if (*this->_peekChar() == "=") {
            token = this->_newToken(token::tokenType::EqualEqual, this->currentChar + *this->_peekChar());
            this->_readChar();
        } else {
            token = this->_newToken(token::tokenType::Equals, this->currentChar);
        }
    } else if (this->currentChar == ">") {
        if (*this->_peekChar() == "=") {
            token = this->_newToken(token::tokenType::GreaterThanOrEqual, this->currentChar + *this->_peekChar());
            this->_readChar();
        } else if (*this->_peekChar() == ">") {
            token = this->_newToken(token::tokenType::RightShift, this->currentChar + *this->_peekChar());
            this->_readChar();
        } else {
            token = this->_newToken(token::tokenType::GreaterThan, this->currentChar);
        }
    } else if (this->currentChar == "<") {
        if (*this->_peekChar() == "=") {
            token = this->_newToken(token::tokenType::LessThanOrEqual, this->currentChar + *this->_peekChar());
            this->_readChar();
        } else if (*this->_peekChar() == "<") {
            token = this->_newToken(token::tokenType::LeftShift, this->currentChar + *this->_peekChar());
            this->_readChar();
        } else {
            token = this->_newToken(token::tokenType::LessThan, this->currentChar);
        }
    } else if (this->currentChar == "!") {
        if (*this->_peekChar() == "=") {
            token = this->_newToken(token::tokenType::NotEquals, this->currentChar + *this->_peekChar());
            this->_readChar();
        } else {
            token = this->_newToken(token::tokenType::Illegal, this->currentChar);
        }
    } else if (this->currentChar == "{") {
        token = this->_newToken(token::tokenType::LeftBrace, this->currentChar);
    } else if (this->currentChar == "}") {
        token = this->_newToken(token::tokenType::RightBrace, this->currentChar);
    } else if (this->currentChar == "(") {
        token = this->_newToken(token::tokenType::LeftParen, this->currentChar);
    } else if (this->currentChar == ")") {
        token = this->_newToken(token::tokenType::RightParen, this->currentChar);
    } else if (this->currentChar == "{") {
        token = this->_newToken(token::tokenType::LeftBrace, this->currentChar);
    } else if (this->currentChar == "}") {
        token = this->_newToken(token::tokenType::RightBrace, this->currentChar);
    } else if (this->currentChar == ":") {
        token = this->_newToken(token::tokenType::Colon, this->currentChar);
    } else if (this->currentChar == ";") {
        token = this->_newToken(token::tokenType::Semicolon, this->currentChar);
    } else if (this->currentChar == "&") {
        if (*this->_peekChar() == "&") {
            token = this->_newToken(token::tokenType::BitwiseAnd, this->currentChar + *this->_peekChar());
            this->_readChar();
        } else {
            token = this->_newToken(token::tokenType::Illegal, this->currentChar);
        }
    } else if (this->currentChar == "|") {
        if (*this->_peekChar() == "|") {
            token = this->_newToken(token::tokenType::BitwiseOr, this->currentChar + *this->_peekChar());
            this->_readChar();
        } else {
            token = this->_newToken(token::tokenType::Illegal, this->currentChar);
        }
    } else if (this->currentChar == "~") {
        token = this->_newToken(token::tokenType::BitwiseNot, this->currentChar);
    } else if (this->currentChar == ",") {
        token = this->_newToken(token::tokenType::Comma, this->currentChar);
    } else if (this->currentChar == "") {
        token = this->_newToken(token::tokenType::EndOfFile, "");
    } else {
        if (*this->_isString() != "") {
            std::shared_ptr<std::string> str = this->_readString(*this->_isString());
            token = this->_newToken(token::tokenType::String, *str);
            return token;
        } else if (this->_isLetter(this->currentChar)) {
            std::shared_ptr<std::string> ident = this->_readIdentifier();
            token = this->_newToken(this->_lookupIdent(ident), *ident);
            return token;
        } else if (this->_isDigit(this->currentChar)) {
            token = this->_readNumber();
            return token;
        } else {
            token = this->_newToken(token::tokenType::Illegal, this->currentChar);
        }
    }

    this->_readChar();
    return token;
}
