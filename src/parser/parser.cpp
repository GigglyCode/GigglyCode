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
    if (this->_currentTokenIs(token::tokenType::Identifier))
    {
        if (this->_peekTokenIs(token::tokenType::Colon))
        {
            return this->_parseVariableDeclaration();
        }
        else if (this->_peekTokenIs(token::tokenType::Equals))
        {
            return this->_parseVariableAssignment();
        }
        else
        {
            return this->_parseExpressionStatement();
        }
    }
    else if (this->_currentTokenIs(token::tokenType::LeftBrace))
    {
        return this->_parseBlockStatement();
    }
    else if (this->_currentTokenIs(token::tokenType::Return))
    {
        return this->_parseReturnStatement();
    }
    else if (this->_currentTokenIs(token::tokenType::Def))
    {
        return this->_parseFunctionStatement();
    }
    else
        return this->_parseExpressionStatement();
}

std::shared_ptr<AST::functionStatement> parser::Parser::_parseFunctionStatement()
{
    if (!this->_expectPeek(token::tokenType::Identifier))
    {
        return nullptr;
    }
    auto name = std::make_shared<AST::identifierLiteral>(this->current_token->literal);
    if (!this->_expectPeek(token::tokenType::LeftParen))
    {
        return nullptr;
    }
    this->_nextToken();
    std::vector<std::shared_ptr<AST::functionStatement::parameter>> parameters;
    while (this->current_token->type != token::tokenType::RightParen)
    {
        std::cout << (this->current_token->type == token::tokenType::Identifier);
        if (this->current_token->type == token::tokenType::Identifier)
        {
            auto identifier = std::make_shared<AST::identifierLiteral>(this->current_token->literal);
            if (!this->_expectPeek(token::tokenType::Colon))
            {
                return nullptr;
            }
            this->_nextToken();
            auto type = this->_parseType();
            parameters.push_back(std::make_shared<AST::functionStatement::parameter>(identifier, type));
            this->_nextToken();
            if (this->current_token->type == token::tokenType::Comma)
            {
                this->_nextToken();
                continue;
            }
            else if (this->current_token->type == token::tokenType::RightParen)
            {
                break;
            }
            else
            {
                _peekError(current_token->type, token::tokenType::Comma);
                break;
            }
        }
        else
        {
            _peekError(current_token->type, token::tokenType::Identifier);
            break;
        }
    }
    if (!this->_expectPeek(token::tokenType::RightArrow))
    {
        return nullptr;
    }
    this->_nextToken();
    auto returnType = this->_parseType();
    auto body = this->_parseStatement();
    return std::make_shared<AST::functionStatement>(name, parameters, returnType, body);
}

std::shared_ptr<AST::returnStatement> parser::Parser::_parseReturnStatement()
{
    this->_nextToken();
    auto expr = this->_parseExpression(PrecedenceType::LOWEST);
    if (this->_peekTokenIs(token::tokenType::Semicolon))
    {
        this->_nextToken();
    }
    return std::make_shared<AST::returnStatement>(expr);
}

std::shared_ptr<AST::blockStatement> parser::Parser::_parseBlockStatement()
{
    this->_nextToken();
    std::vector<std::shared_ptr<AST::statement>> statements;
    while (!this->_currentTokenIs(token::tokenType::RightBrace) && !this->_currentTokenIs(token::tokenType::EndOfFile))
    {
        auto stmt = this->_parseStatement();
        if (stmt != nullptr)
        {
            statements.push_back(stmt);
        }
        this->_nextToken();
    }
    return std::make_shared<AST::blockStatement>(statements);
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

std::shared_ptr<AST::statement> parser::Parser::_parseVariableDeclaration()
{
    auto identifier = std::make_shared<AST::identifierLiteral>(current_token->literal);
    this->_nextToken();
    this->_nextToken();
    auto type = this->_parseType();
    if (this->peek_token->type == token::tokenType::Semicolon)
    {
        this->_nextToken();
        return std::make_shared<AST::variableDeclarationStatement>(identifier, type);
    }
    else if (this->_expectPeek(token::tokenType::Equals))
    {
        this->_nextToken();
        auto expr = this->_parseExpression(PrecedenceType::LOWEST);
        this->_nextToken();
        return std::make_shared<AST::variableDeclarationStatement>(identifier, type, expr);
    }
    return nullptr;
}

std::shared_ptr<AST::baseType> parser::Parser::_parseType()
{
    std::shared_ptr<AST::expression> name;
    if (this->_currentTokenIs(token::tokenType::Integer))
        name = std::make_shared<AST::integerLiteral>(std::stoll(this->current_token->literal));
    else if (this->_currentTokenIs(token::tokenType::Float))
        name = std::make_shared<AST::floatLiteral>(std::stod(this->current_token->literal));
    else if (this->_currentTokenIs(token::tokenType::String))
        name = std::make_shared<AST::stringLiteral>(this->current_token->literal);
    else if (this->_currentTokenIs(token::tokenType::RawString))
        name = std::make_shared<AST::stringLiteral>(this->current_token->literal);
    else
        name = std::make_shared<AST::identifierLiteral>(current_token->literal);
    std::vector<std::shared_ptr<AST::baseType>> generics;
    if (this->_peekTokenIs(token::tokenType::LeftBracket))
    {
        this->_nextToken();
        this->_nextToken();
        while (this->current_token->type != token::tokenType::RightBracket)
        {
            auto generic = this->_parseType();
            generics.push_back(generic);
            this->_nextToken();
            if (this->current_token->type == token::tokenType::Comma)
            {
                this->_nextToken();
            }
        }
    }
    return std::make_shared<AST::genericType>(name, generics);
}

std::shared_ptr<AST::statement> parser::Parser::_parseVariableAssignment()
{
    auto identifier = std::make_shared<AST::identifierLiteral>(current_token->literal);
    if (!this->_expectPeek(token::tokenType::Equals))
    {
        return nullptr;
    }
    this->_nextToken();
    auto expr = this->_parseExpression(PrecedenceType::LOWEST);
    this->_nextToken();
    auto stmt = std::make_shared<AST::variableAssignmentStatement>(identifier, expr);
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

std::shared_ptr<AST::expression> parser::Parser::_parseStringLiteral()
{
    auto expr = std::make_shared<AST::stringLiteral>(current_token->literal);
    return expr;
}

std::shared_ptr<AST::expression> parser::Parser::_parseIdentifier()
{
    auto expr = std::make_shared<AST::identifierLiteral>(current_token->literal);
    return expr;
}

void parser::Parser::_nextToken()
{
    current_token = peek_token;
    peek_token = lexer->nextToken();
}

bool parser::Parser::_currentTokenIs(token::tokenType type)
{
    return current_token->type == type;
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