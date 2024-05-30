#include "parser.hpp"

parser::Parser::Parser(std::shared_ptr<Lexer> lexer) {
    this->lexer = lexer;
    current_token = nullptr;
    peek_token = nullptr;
    this->_nextToken();
    this->_nextToken();
}

std::shared_ptr<AST::Program> parser::Parser::parseProgram() {
    std::shared_ptr<AST::Program> program = std::make_shared<AST::Program>();
    while(current_token->type != token::TokenType::EndOfFile) {
        auto statement = this->_parseStatement();
        if(statement != nullptr) {
            program->statements.push_back(statement);
        }
        this->_nextToken();
    }
    return program;
}

std::shared_ptr<AST::Statement> parser::Parser::_parseStatement() {
    if(this->_currentTokenIs(token::TokenType::Identifier)) {
        if(this->_peekTokenIs(token::TokenType::Colon)) {
            return this->_parseVariableDeclaration();
        } else if(this->_peekTokenIs(token::TokenType::Equals)) {
            return this->_parseVariableAssignment();
        } else {
            return this->_parseExpressionStatement();
        }
    } else if(this->_currentTokenIs(token::TokenType::LeftBrace)) {
        return this->_parseBlockStatement();
    } else if(this->_currentTokenIs(token::TokenType::Return)) {
        return this->_parseReturnStatement();
    } else if(this->_currentTokenIs(token::TokenType::Def)) {
        return this->_parseFunctionStatement();
    } else if(this->_currentTokenIs(token::TokenType::If)) {
        return this->_parseIfElseStatement();
    } else
        return this->_parseExpressionStatement();
}

std::shared_ptr<AST::FunctionStatement> parser::Parser::_parseFunctionStatement() {
    if(!this->_expectPeek(token::TokenType::Identifier)) {
        return nullptr;
    }
    auto name = std::make_shared<AST::IdentifierLiteral>(this->current_token->literal);
    if(!this->_expectPeek(token::TokenType::LeftParen)) {
        return nullptr;
    }
    this->_nextToken();
    std::vector<std::shared_ptr<AST::FunctionParameter>> parameters;
    while(this->current_token->type != token::TokenType::RightParen) {
        if(this->current_token->type == token::TokenType::Identifier) {
            auto identifier = std::make_shared<AST::IdentifierLiteral>(this->current_token->literal);
            if(!this->_expectPeek(token::TokenType::Colon)) {
                return nullptr;
            }
            this->_nextToken();
            auto type = this->_parseType();
            parameters.push_back(std::make_shared<AST::FunctionParameter>(identifier, type));
            this->_nextToken();
            if(this->current_token->type == token::TokenType::Comma) {
                this->_nextToken();
                continue;
            } else if(this->current_token->type == token::TokenType::RightParen) {
                break;
            } else {
                _peekError(current_token->type, token::TokenType::Comma);
                break;
            }
        } else {
            _peekError(current_token->type, token::TokenType::Identifier);
            break;
        }
    }
    if(!this->_expectPeek(token::TokenType::RightArrow)) {
        return nullptr;
    }
    this->_nextToken();
    auto returnType = this->_parseType();
    if(!this->_expectPeek(token::TokenType::LeftBrace)) {
        return nullptr;
    }
    auto body = this->_parseBlockStatement();
    return std::make_shared<AST::FunctionStatement>(name, parameters, returnType, body);
}

std::shared_ptr<AST::Expression> parser::Parser::_parseFunctionCall() {
    auto identifier = std::make_shared<AST::IdentifierLiteral>(current_token->literal);
    this->_nextToken();
    auto args = this->_parse_expression_list(token::TokenType::RightParen);
    return std::make_shared<AST::CallExpression>(identifier, args);
}

std::vector<std::shared_ptr<AST::Expression>> parser::Parser::_parse_expression_list(token::TokenType end) {
    std::vector<std::shared_ptr<AST::Expression>> args;
    if(this->_peekTokenIs(end)) {
        this->_nextToken();
        return args;
    }
    this->_nextToken();
    args.push_back(this->_parseExpression(PrecedenceType::LOWEST));
    while(this->_peekTokenIs(token::TokenType::Comma)) {
        this->_nextToken();
        this->_nextToken();
        args.push_back(this->_parseExpression(PrecedenceType::LOWEST));
    }
    if(!this->_expectPeek(end)) {
        return std::vector<std::shared_ptr<AST::Expression>>{};
    }
    return args;
}

std::shared_ptr<AST::ReturnStatement> parser::Parser::_parseReturnStatement() {
    this->_nextToken();
    auto expr = this->_parseExpression(PrecedenceType::LOWEST);
    if(this->_peekTokenIs(token::TokenType::Semicolon)) {
        this->_nextToken();
    }
    return std::make_shared<AST::ReturnStatement>(expr);
}

std::shared_ptr<AST::BlockStatement> parser::Parser::_parseBlockStatement() {
    this->_nextToken();
    std::vector<std::shared_ptr<AST::Statement>> statements;
    while(!this->_currentTokenIs(token::TokenType::RightBrace) && !this->_currentTokenIs(token::TokenType::EndOfFile)) {
        auto stmt = this->_parseStatement();
        if(stmt != nullptr) {
            statements.push_back(stmt);
        }
        this->_nextToken();
    }
    return std::make_shared<AST::BlockStatement>(statements);
}

std::shared_ptr<AST::ExpressionStatement> parser::Parser::_parseExpressionStatement() {
    auto expr = this->_parseExpression(PrecedenceType::LOWEST);
    if(this->_peekTokenIs(token::TokenType::Semicolon)) {
        this->_nextToken();
    }
    auto stmt = std::make_shared<AST::ExpressionStatement>(expr);
    return stmt;
}

std::shared_ptr<AST::Statement> parser::Parser::_parseVariableDeclaration() {
    auto identifier = std::make_shared<AST::IdentifierLiteral>(current_token->literal);
    this->_nextToken();
    this->_nextToken();
    auto type = this->_parseType();
    if(this->peek_token->type == token::TokenType::Semicolon) {
        this->_nextToken();
        return std::make_shared<AST::VariableDeclarationStatement>(identifier, type);
    } else if(this->_expectPeek(token::TokenType::Equals)) {
        this->_nextToken();
        auto expr = this->_parseExpression(PrecedenceType::LOWEST);
        this->_nextToken();
        return std::make_shared<AST::VariableDeclarationStatement>(identifier, type, expr);
    }
    return nullptr;
}

std::shared_ptr<AST::BaseType> parser::Parser::_parseType() {
    std::shared_ptr<AST::Expression> name;
    if(this->_currentTokenIs(token::TokenType::Integer))
        name = std::make_shared<AST::IntegerLiteral>(std::stoll(this->current_token->literal));
    else if(this->_currentTokenIs(token::TokenType::Float))
        name = std::make_shared<AST::FloatLiteral>(std::stod(this->current_token->literal));
    else if(this->_currentTokenIs(token::TokenType::String))
        name = std::make_shared<AST::StringLiteral>(this->current_token->literal);
    else if(this->_currentTokenIs(token::TokenType::RawString))
        name = std::make_shared<AST::StringLiteral>(this->current_token->literal);
    else
        name = std::make_shared<AST::IdentifierLiteral>(current_token->literal);
    std::vector<std::shared_ptr<AST::BaseType>> generics;
    if(this->_peekTokenIs(token::TokenType::LeftBracket)) {
        this->_nextToken();
        this->_nextToken();
        while(this->current_token->type != token::TokenType::RightBracket) {
            auto generic = this->_parseType();
            generics.push_back(generic);
            this->_nextToken();
            if(this->current_token->type == token::TokenType::Comma) {
                this->_nextToken();
            }
        }
    }
    return std::make_shared<AST::GenericType>(name, generics);
}

std::shared_ptr<AST::Statement> parser::Parser::_parseVariableAssignment() {
    auto identifier = std::make_shared<AST::IdentifierLiteral>(current_token->literal);
    if(!this->_expectPeek(token::TokenType::Equals)) {
        return nullptr;
    }
    this->_nextToken();
    auto expr = this->_parseExpression(PrecedenceType::LOWEST);
    this->_nextToken();
    auto stmt = std::make_shared<AST::VariableAssignmentStatement>(identifier, expr);
    return stmt;
}

std::shared_ptr<AST::Expression> parser::Parser::_parseExpression(PrecedenceType precedence) {
    auto iter = prefix_parse_fns.find(current_token->type);
    if(iter == prefix_parse_fns.end()) {
        this->_noPrefixParseFnError(current_token->type);
        return nullptr;
    }
    auto prefix_fn = iter->second;
    auto leftExpr = prefix_fn();
    while(!_peekTokenIs(token::TokenType::Semicolon) && precedence < _peekPrecedence()) {
        auto iter = infix_parse_Fns.find(peek_token->type);
        if(iter == infix_parse_Fns.end()) {
            return leftExpr;
        }
        this->_nextToken();
        auto infix_fn = iter->second;
        leftExpr = infix_fn(leftExpr);
    }
    return leftExpr;
}

std::shared_ptr<AST::Statement> parser::Parser::_parseIfElseStatement() {
    if(!this->_expectPeek(token::TokenType::LeftParen)) {
        return nullptr;
    }
    this->_nextToken();
    auto condition = this->_parseExpression(PrecedenceType::LOWEST);
    if(!this->_expectPeek(token::TokenType::RightParen)) {
        return nullptr;
    }
    this->_nextToken();
    auto consequence = this->_parseStatement();
    std::shared_ptr<AST::Statement> alternative = nullptr;
    if(this->_peekTokenIs(token::TokenType::Else)) {
        this->_nextToken();
        this->_nextToken();
        alternative = this->_parseStatement();
    }
    return std::make_shared<AST::IfElseStatement>(condition, consequence, alternative);
}

std::shared_ptr<AST::Expression> parser::Parser::_parseInfixExpression(std::shared_ptr<AST::Expression> leftNode) {
    auto infixExpr = std::make_shared<AST::InfixExpression>(leftNode, this->current_token->type);
    auto precedence = this->_currentPrecedence();
    this->_nextToken();
    infixExpr->right = this->_parseExpression(precedence);
    return infixExpr;
}

std::shared_ptr<AST::Expression> parser::Parser::_parseGroupedExpression() {
    this->_nextToken();
    auto expr = this->_parseExpression(PrecedenceType::LOWEST);
    if(!this->_expectPeek(token::TokenType::RightParen)) {
        return nullptr;
    }
    return expr;
}


std::shared_ptr<AST::Expression> parser::Parser::_parseIntegerLiteral() {
    auto expr = std::make_shared<AST::IntegerLiteral>(std::stoll(current_token->literal));
    return expr;
}

std::shared_ptr<AST::Expression> parser::Parser::_parseFloatLiteral() {
    auto expr = std::make_shared<AST::FloatLiteral>(std::stod(current_token->literal));
    return expr;
}

std::shared_ptr<AST::Expression> parser::Parser::_parseBooleanLiteral() {
    auto expr = std::make_shared<AST::BooleanLiteral>(current_token->type == token::TokenType::True);
    return expr;
}

std::shared_ptr<AST::Expression> parser::Parser::_parseStringLiteral() {
    auto expr = std::make_shared<AST::StringLiteral>(current_token->literal);
    return expr;
}

std::shared_ptr<AST::Expression> parser::Parser::_parseIdentifier() {
    if(this->_peekTokenIs(token::TokenType::LeftParen)) {
        return this->_parseFunctionCall();
    }
    auto expr = std::make_shared<AST::IdentifierLiteral>(current_token->literal);
    return expr;
}

void parser::Parser::_nextToken() {
    current_token = peek_token;
    peek_token = lexer->nextToken();
}

bool parser::Parser::_currentTokenIs(token::TokenType type) { return current_token->type == type; }

bool parser::Parser::_peekTokenIs(token::TokenType type) { return peek_token->type == type; }

bool parser::Parser::_expectPeek(token::TokenType type) {
    if(_peekTokenIs(type)) {
        _nextToken();
        return true;
    } else {
        _peekError(peek_token->type, type);
        return false;
    }
}

parser::PrecedenceType parser::Parser::_currentPrecedence() {
    auto it = token_precedence.find(current_token->type);
    if(it != token_precedence.end()) {
        return it->second;
    } else {
        return PrecedenceType::LOWEST;
    }
}

parser::PrecedenceType parser::Parser::_peekPrecedence() {
    auto it = token_precedence.find(peek_token->type);
    if(it != token_precedence.end()) {
        return it->second;
    } else {
        return PrecedenceType::LOWEST;
    }
}

void parser::Parser::_peekError(token::TokenType type, token::TokenType expected_type, std::string suggestedFix) {
    std::shared_ptr<errors::SyntaxError> error = std::make_shared<errors::SyntaxError>(
        this->lexer->source, *peek_token,
        "Expected next token to be " + *token::tokenTypeString(expected_type) + ", got " + *token::tokenTypeString(type), suggestedFix);
    this->errors.push_back(error);
}

void parser::Parser::_noPrefixParseFnError(token::TokenType type) {
    // std::shared_ptr<errors::NoPrefixParseFnError> error = std::make_shared<errors::NoPrefixParseFnError>(
    //     this->lexer->source, *peek_token, "No prefix parse function for " + *token::tokenTypeString(type));
    // this->errors.push_back(error);
}