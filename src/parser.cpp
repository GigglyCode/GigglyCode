#include "include/parser.hpp"
#include <sstream>

std::unordered_map<tokenType, precidanceType> precidences = {
    {tokenType::INT, precidanceType::LOWEST},
    {tokenType::FLOAT, precidanceType::LOWEST},
    {tokenType::PLUS, precidanceType::SUM},
    {tokenType::MINUS, precidanceType::SUM},
    {tokenType::ASTERISK, precidanceType::PRODUCT},
    {tokenType::FSLASH, precidanceType::PRODUCT},
    {tokenType::BSLASH, precidanceType::PRODUCT},
    {tokenType::PERCENT, precidanceType::PRODUCT},
    {tokenType::CARET, precidanceType::EXPONENT},
    {tokenType::LPAREN, precidanceType::CALL},
    {tokenType::RPAREN, precidanceType::CALL},
    {tokenType::SEMICOLON, precidanceType::LOWEST},
    {tokenType::ILLEGAL, precidanceType::LOWEST},
    {tokenType::END, precidanceType::LOWEST},
};

Parser::Parser(std::shared_ptr<Lexer> lexer)
{
    this->lexer = lexer;

    this->_nextToken();
    this->_nextToken();
}

void Parser::_nextToken()
{
    this->currentToken = this->peekToken;
    this->peekToken = this->lexer->nextToken();
}

bool Parser::_peekTokenIs(tokenType type)
{
    return this->peekToken->type == type;
}

bool Parser::_currentTokenIs(tokenType type)
{
    return this->currentToken->type == type;
}

bool Parser::_expectPeek(tokenType type)
{
    if (this->_peekTokenIs(type))
    {
        this->_nextToken();
        return true;
    }
    else
    {
        this->_peekError(type);
        return false;
    }
}

precidanceType Parser::_currentPrecedence()
{
    if (precidences.find(this->currentToken->type) != precidences.end())
    {
        return precidences[this->currentToken->type];
    }
    return precidanceType::LOWEST;
}

precidanceType Parser::_peekPrecedence()
{
    if (precidences.find(this->peekToken->type) != precidences.end())
    {
        return precidences[this->peekToken->type];
    }
    return precidanceType::LOWEST;
}

void Parser::_peekError(tokenType type)
{
    std::string error = "expected next token to be " + tokenTypeString(type) + ", got " + tokenTypeString(this->peekToken->type) + " instead";
    this->errors.push_back(std::make_shared<std::string>(error));
}

void Parser::_noPrefixParseFnError(tokenType type)
{
    std::string error = "no prefix parse function for " + tokenTypeString(type) + " found";
    this->errors.push_back(std::make_shared<std::string>(error));
}

std::shared_ptr<AST::Program> Parser::parseProgram()
{
    AST::Program program;
    do
    {
        std::shared_ptr<AST::Statement> stmt = this->_parseStatement();
        if (stmt != nullptr)
        {
            program.statements.push_back(stmt);
        }
        this->_nextToken();
    } while (this->currentToken->type != tokenType::END);
    return std::make_shared<AST::Program>(program);
}

std::shared_ptr<AST::Statement> Parser::_parseStatement()
{
    switch (this->currentToken->type)
    {
    case tokenType::LET:
        return this->_parseVariableDeclarationStatement();
        break;
    default:
        return this->_parseExpressionStatement();
        break;
    }
}

std::shared_ptr<AST::ExpressionStatement> Parser::_parseExpressionStatement()
{
    std::shared_ptr<AST::Expression> expr = this->_parseExpression(precidanceType::LOWEST);

    if (this->_peekTokenIs(tokenType::SEMICOLON))
    {
        this->_nextToken();
    }

    AST::ExpressionStatement stmt(expr);
    return std::make_shared<AST::ExpressionStatement>(stmt);
}

std::shared_ptr<AST::VariableDeclarationStatement> Parser::_parseVariableDeclarationStatement()
{
    AST::VariableDeclarationStatement stmt;
    if (!this->_expectPeek(tokenType::IDENT))
    {
        return nullptr;
    }
    stmt.name = std::make_shared<AST::IdentifierLiteral>(AST::IdentifierLiteral(this->currentToken->literal));
    if (!this->_expectPeek(tokenType::COLON))
    {
        return nullptr;
    }
    if (!this->_expectPeek(tokenType::IDENT))
    {
        return nullptr;
    }
    stmt.valueType = std::make_shared<AST::IdentifierLiteral>(AST::IdentifierLiteral(this->currentToken->literal));
    if (!this->_expectPeek(tokenType::EQUALS))
    {
        return nullptr;
    }
    this->_nextToken();
    stmt.value = this->_parseExpression(precidanceType::LOWEST);
    while (!(this->currentToken->type == tokenType::SEMICOLON))
    {
        this->_nextToken();
    }
    return std::make_shared<AST::VariableDeclarationStatement>(stmt);
}

std::shared_ptr<AST::Expression> Parser::_parseExpression(precidanceType precedence)
{
    auto prefixFn = this->prefixParseFns[this->currentToken->type];
    if (prefixFn == nullptr)
    {
        this->_noPrefixParseFnError(this->currentToken->type);
        return nullptr;
    }
    std::shared_ptr<AST::Expression> leftExp = prefixFn();
    while (!this->_peekTokenIs(tokenType::SEMICOLON) && precedence < this->_peekPrecedence())
    {
        auto infixFn = this->infixParseFns[this->peekToken->type];
        if (infixFn == nullptr)
        {
            return leftExp;
        }
        this->_nextToken();
        leftExp = infixFn(leftExp);
    }
    return leftExp;
}

std::shared_ptr<AST::Expression> Parser::_parseInfixExpression(std::shared_ptr<AST::Expression> left_node)
{
    AST::InfixExpression infix = AST::InfixExpression(left_node, this->currentToken->literal, nullptr);
    precidanceType precedence = this->_currentPrecedence();
    this->_nextToken();
    infix.right = this->_parseExpression(precedence);
    return std::make_shared<AST::InfixExpression>(infix);
}

std::shared_ptr<AST::Expression> Parser::_parsegroupedExpression()
{
    this->_nextToken();
    std::shared_ptr<AST::Expression> exp = this->_parseExpression(precidanceType::LOWEST);
    if (!this->_expectPeek(tokenType::RPAREN))
    {
        return nullptr;
    }
    return exp;
}

std::shared_ptr<AST::Expression> Parser::_parseIntegerLiteral()
{
    try
    {
        AST::IntegerLiteral lit = AST::IntegerLiteral(std::stoi(this->currentToken->literal));
        return std::make_shared<AST::IntegerLiteral>(lit);
    }
    catch (const std::invalid_argument &e)
    {
        std::string error = "could not parse " + this->currentToken->literal + " as integer";
        this->errors.push_back(std::make_shared<std::string>(error));
        return nullptr;
    }
}

std::shared_ptr<AST::Expression> Parser::_parseFloatLiteral()
{
    try
    {
        AST::FloatLiteral lit = AST::FloatLiteral(std::stof(this->currentToken->literal));
        return std::make_shared<AST::FloatLiteral>(lit);
    }
    catch (const std::invalid_argument &e)
    {
        std::string error = "could not parse " + this->currentToken->literal + " as float";
        this->errors.push_back(std::make_shared<std::string>(error));
        return nullptr;
    }
}