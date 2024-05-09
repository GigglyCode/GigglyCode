#include "parser.hpp"

parser::Parser::Parser(std::shared_ptr<Lexer> lexer)
{
    this->lexer = lexer;
    current_token = nullptr;
    peek_token = nullptr;
    this->_nextToken();
    this->_nextToken();
}

std::shared_ptr<AST::program> parser::Parser::parseProgram()
{
    std::shared_ptr<AST::program> program = std::make_shared<AST::program>();
    while (current_token->type != token::tokenType::EndOfFile)
    {
        auto statement = this->_parseStatement();
        if (statement != nullptr)
        {
            program->statements.push_back(statement);
        }
        this->_nextToken();
    }
    return program;
}

std::shared_ptr<AST::statement> parser::Parser::_parseStatement()
{
    return this->_parseExpressionStatement();
}

std::shared_ptr<AST::expressionStatement> parser::Parser::_parseExpressionStatement()
{
    auto expr = this->_parseExpression(PrecedenceType::LOWEST);
    if (this->_peekTokenIs(token::tokenType::Semicolon))
    {
        this->_nextToken();
    }
    auto stmt = std::make_shared<AST::expressionStatement>(expr);
    return stmt;
}

std::shared_ptr<AST::expression> parser::Parser::_parseExpression(PrecedenceType precedence)
{
    auto iter = prefixParseFns.find(current_token->type);
    if (iter == prefixParseFns.end())
    {
        this->_noPrefixParseFnError(current_token->type);
        return nullptr;
    }
    auto prefix_fn = iter->second;
    auto leftExpr = prefix_fn();
    while (!_peekTokenIs(token::tokenType::Semicolon) && precedence < _peekPrecedence())
    {
        auto iter = infixParseFns.find(peek_token->type);
        if (iter == infixParseFns.end())
        {
            return leftExpr;
        }
        this->_nextToken();
        auto infix_fn = iter->second;
        leftExpr = infix_fn(leftExpr);
    }
    return leftExpr;
}

std::shared_ptr<AST::expression> parser::Parser::_parseInfixExpression(std::shared_ptr<AST::expression> leftNode)
{
    auto infixExpr = std::make_shared<AST::infixExpression>(leftNode, this->current_token->type);
    auto precedence = this->_currentPrecedence();
    this->_nextToken();
    infixExpr->right = this->_parseExpression(precedence);
    return infixExpr;
}

std::shared_ptr<AST::expression> parser::Parser::_parseGroupedExpression()
{
    this->_nextToken();
    auto expr = this->_parseExpression(PrecedenceType::LOWEST);
    if (!this->_expectPeek(token::tokenType::RightParen))
    {
        return nullptr;
    }
    return expr;
}

std::shared_ptr<AST::expression> parser::Parser::_parseIntegerLiteral()
{
    auto expr = std::make_shared<AST::integerLiteral>(std::stoll(current_token->literal));
    return expr;
}

std::shared_ptr<AST::expression> parser::Parser::_parseFloatLiteral()
{
    auto expr = std::make_shared<AST::floatLiteral>(std::stod(current_token->literal));
    return expr;
}

void parser::Parser::_nextToken()
{
    current_token = peek_token;
    peek_token = lexer->nextToken();
}

bool parser::Parser::_peekTokenIs(token::tokenType type)
{
    return peek_token->type == type;
}

bool parser::Parser::_expectPeek(token::tokenType type)
{
    if (_peekTokenIs(type))
    {
        _nextToken();
        return true;
    }
    else
    {
        _peekError(peek_token->type, type);
        return false;
    }
}

parser::PrecedenceType parser::Parser::_currentPrecedence()
{
    auto it = token_precedence.find(current_token->type);
    if (it != token_precedence.end())
    {
        return it->second;
    }
    else
    {
        return PrecedenceType::LOWEST;
    }
}

parser::PrecedenceType parser::Parser::_peekPrecedence()
{
    auto it = token_precedence.find(peek_token->type);
    if (it != token_precedence.end())
    {
        return it->second;
    }
    else
    {
        return PrecedenceType::LOWEST;
    }
}

void parser::Parser::_peekError(token::tokenType type, token::tokenType expected_type, std::string suggestedFix)
{
    std::shared_ptr<errors::SyntaxError> error = std::make_shared<errors::SyntaxError>(this->lexer->source, *peek_token, "Expected next token to be " + *token::tokenTypeString(expected_type) + ", got " + *token::tokenTypeString(type), suggestedFix);
    this->errors.push_back(error);
}

void parser::Parser::_noPrefixParseFnError(token::tokenType type)
{
    std::shared_ptr<errors::NoPrefixParseFnError> error = std::make_shared<errors::NoPrefixParseFnError>(this->lexer->source, *peek_token, "No prefix parse function for " + *token::tokenTypeString(type));
    this->errors.push_back(error);
}