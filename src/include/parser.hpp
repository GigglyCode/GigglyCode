#ifndef PARSER_HPP
#define PARSER_HPP
#include <unordered_map>
#include <vector>
#include "lexer.hpp"
#include "AST.hpp"

enum class precidanceType
{
    LOWEST,
    EQUALS,
    LESSGREATER,
    SUM,
    PRODUCT,
    EXPONENT,
    PREFIX,
    CALL,
    INDEX
};

class Parser
{
public:
    std::shared_ptr<Lexer> lexer;
    std::vector<std::shared_ptr<std::string>> errors;
    std::shared_ptr<Token> currentToken = nullptr;
    std::shared_ptr<Token> peekToken = nullptr;
    std::unordered_map<tokenType, std::function<std::shared_ptr<AST::Expression>()>> prefixParseFns = {
        {tokenType::INT, std::bind(&Parser::_parseIntegerLiteral, this)},
        {tokenType::FLOAT, std::bind(&Parser::_parseFloatLiteral, this)},
        {tokenType::LPAREN, std::bind(&Parser::_parsegroupedExpression, this)},
    };
    std::unordered_map<tokenType, std::function<std::shared_ptr<AST::Expression>(std::shared_ptr<AST::Expression>)>> infixParseFns = {
        {tokenType::PLUS, std::bind(&Parser::_parseInfixExpression, this, std::placeholders::_1)},
        {tokenType::MINUS, std::bind(&Parser::_parseInfixExpression, this, std::placeholders::_1)},
        {tokenType::ASTERISK, std::bind(&Parser::_parseInfixExpression, this, std::placeholders::_1)},
        {tokenType::FSLASH, std::bind(&Parser::_parseInfixExpression, this, std::placeholders::_1)},
        {tokenType::BSLASH, std::bind(&Parser::_parseInfixExpression, this, std::placeholders::_1)},
        {tokenType::PERCENT, std::bind(&Parser::_parseInfixExpression, this, std::placeholders::_1)},
        {tokenType::CARET, std::bind(&Parser::_parseInfixExpression, this, std::placeholders::_1)},
        {tokenType::LPAREN, std::bind(&Parser::_parseInfixExpression, this, std::placeholders::_1)},
    };
    Parser(std::shared_ptr<Lexer> lexer);
    std::shared_ptr<AST::Program> parseProgram();

private:
    void _nextToken();
    bool _peekTokenIs(tokenType type);
    bool _currentTokenIs(tokenType type);
    bool _expectPeek(tokenType type);
    void _peekError(tokenType type);
    void _noPrefixParseFnError(tokenType type);
    precidanceType _currentPrecedence();
    precidanceType _peekPrecedence();
    std::shared_ptr<AST::Statement> _parseStatement();
    std::shared_ptr<AST::ExpressionStatement> _parseExpressionStatement();
    std::shared_ptr<AST::VariableDeclarationStatement> _parseVariableDeclarationStatement();
    std::shared_ptr<AST::Expression> _parseExpression(precidanceType precedence);

    std::shared_ptr<AST::Expression> _parseInfixExpression(std::shared_ptr<AST::Expression> left_node);
    std::shared_ptr<AST::Expression> _parsegroupedExpression();
    std::shared_ptr<AST::Expression> _parseIntegerLiteral();
    std::shared_ptr<AST::Expression> _parseFloatLiteral();
};

#endif
