#ifndef PARSER_HPP
#define PARSER_HPP
#include "../errors/errors.hpp"
#include "../lexer/lexer.hpp"
#include "../lexer/tokens.hpp"
#include "AST/ast.hpp"
#include <memory>
#include <unordered_map>
#include <vector>

namespace parser {

enum class PrecedenceType {
    LOWEST,
    ASSIGN,        // =, +=, -=, *=, /=, %=
    COMPARISION,   // >, <, >=, <=, ==, !=
    SUM,           // +
    PRODUCT,       // *
    Exponent,      // **
    PREFIX,        // -X or !X
    CALL,          // myFunction(X)
    INDEX,         // array[index]
    MEMBER_ACCESS, // object.member
    POSTFIX        // X++
};

static const std::unordered_map<token::tokenType, PrecedenceType>
    token_precedence = {
        {token::tokenType::Illegal, PrecedenceType::LOWEST},
        {token::tokenType::GreaterThan, PrecedenceType::COMPARISION},
        {token::tokenType::LessThan, PrecedenceType::COMPARISION},
        {token::tokenType::GreaterThanOrEqual, PrecedenceType::COMPARISION},
        {token::tokenType::LessThanOrEqual, PrecedenceType::COMPARISION},
        {token::tokenType::EqualEqual, PrecedenceType::COMPARISION},
        {token::tokenType::NotEquals, PrecedenceType::COMPARISION},
        {token::tokenType::PlusEqual, PrecedenceType::ASSIGN},
        {token::tokenType::DashEqual, PrecedenceType::ASSIGN},
        {token::tokenType::AsteriskEqual, PrecedenceType::ASSIGN},
        {token::tokenType::PercentEqual, PrecedenceType::ASSIGN},
        {token::tokenType::CaretEqual, PrecedenceType::ASSIGN},
        {token::tokenType::ForwardSlashEqual, PrecedenceType::ASSIGN},
        {token::tokenType::BackwardSlashEqual, PrecedenceType::ASSIGN},
        {token::tokenType::Equals, PrecedenceType::ASSIGN},
        {token::tokenType::Is, PrecedenceType::ASSIGN},
        {token::tokenType::Increment, PrecedenceType::POSTFIX},
        {token::tokenType::Decrement, PrecedenceType::POSTFIX},
        {token::tokenType::BitwiseAnd, PrecedenceType::COMPARISION},
        {token::tokenType::BitwiseOr, PrecedenceType::COMPARISION},
        {token::tokenType::BitwiseXor, PrecedenceType::COMPARISION},
        {token::tokenType::BitwiseNot, PrecedenceType::PREFIX},
        {token::tokenType::LeftShift, PrecedenceType::COMPARISION},
        {token::tokenType::RightShift, PrecedenceType::COMPARISION},
        {token::tokenType::Dot, PrecedenceType::MEMBER_ACCESS},
        {token::tokenType::Ellipsis, PrecedenceType::LOWEST},
        {token::tokenType::Plus, PrecedenceType::SUM},
        {token::tokenType::Dash, PrecedenceType::SUM},
        {token::tokenType::Asterisk, PrecedenceType::PRODUCT},
        {token::tokenType::Percent, PrecedenceType::PRODUCT},
        {token::tokenType::AsteriskAsterisk, PrecedenceType::Exponent},
        {token::tokenType::ForwardSlash, PrecedenceType::PRODUCT},
        {token::tokenType::BackwardSlash, PrecedenceType::PRODUCT},
        {token::tokenType::EndOfFile, PrecedenceType::LOWEST},
};

class Parser {
  public:
    std::shared_ptr<Lexer> lexer;
    std::shared_ptr<token::Token> current_token;
    std::shared_ptr<token::Token> peek_token;
    std::vector<std::shared_ptr<errors::Error>> errors;
    std::unordered_map<token::tokenType,
                       std::function<std::shared_ptr<AST::expression>()>>
        prefixParseFns = {
            {token::tokenType::Integer,
             std::bind(&Parser::_parseIntegerLiteral, this)},
            {token::tokenType::Float,
             std::bind(&Parser::_parseFloatLiteral, this)},
            {token::tokenType::String,
             std::bind(&Parser::_parseStringLiteral, this)},
            {token::tokenType::Identifier,
             std::bind(&Parser::_parseIdentifier, this)},
            {token::tokenType::LeftParen,
             std::bind(&Parser::_parseGroupedExpression, this)},
    };
    std::unordered_map<token::tokenType,
                       std::function<std::shared_ptr<AST::expression>(
                           std::shared_ptr<AST::expression>)>>
        infixParseFns = {
            {token::tokenType::Plus, std::bind(&Parser::_parseInfixExpression,
                                               this, std::placeholders::_1)},
            {token::tokenType::Dash, std::bind(&Parser::_parseInfixExpression,
                                               this, std::placeholders::_1)},
            {token::tokenType::Asterisk,
             std::bind(&Parser::_parseInfixExpression, this,
                       std::placeholders::_1)},
            {token::tokenType::ForwardSlash,
             std::bind(&Parser::_parseInfixExpression, this,
                       std::placeholders::_1)},
            {token::tokenType::Percent,
             std::bind(&Parser::_parseInfixExpression, this,
                       std::placeholders::_1)},
            {token::tokenType::AsteriskAsterisk,
             std::bind(&Parser::_parseInfixExpression, this,
                       std::placeholders::_1)},
    };
    Parser(std::shared_ptr<Lexer> lexer);
    std::shared_ptr<AST::program> parseProgram();

  private:
    void _nextToken();
    bool _currentTokenIs(token::tokenType type);
    bool _peekTokenIs(token::tokenType type);
    bool _expectPeek(token::tokenType type);
    void _peekError(token::tokenType type, token::tokenType expected_type,
                    std::string suggestedFix = "");
    void _noPrefixParseFnError(token::tokenType type);
    PrecedenceType _currentPrecedence();
    PrecedenceType _peekPrecedence();
    std::shared_ptr<AST::statement> _parseStatement();

    std::shared_ptr<AST::expressionStatement> _parseExpressionStatement();
    std::shared_ptr<AST::statement> _parseVariableDeclaration();
    std::shared_ptr<AST::statement> _parseVariableAssignment();
    std::shared_ptr<AST::returnStatement> _parseReturnStatement();
    std::shared_ptr<AST::functionStatement> _parseFunctionStatement();
    std::shared_ptr<AST::blockStatement> _parseBlockStatement();

    std::shared_ptr<AST::baseType> _parseType();

    std::shared_ptr<AST::expression>
    _parseExpression(PrecedenceType precedence);

    std::shared_ptr<AST::expression> _parseIntegerLiteral();
    std::shared_ptr<AST::expression> _parseFloatLiteral();
    std::shared_ptr<AST::expression> _parseStringLiteral();
    std::shared_ptr<AST::expression> _parseGroupedExpression();
    std::shared_ptr<AST::expression> _parseIdentifier();
    // std::shared_ptr<AST::expression> _parseIdentifier();

    std::shared_ptr<AST::expression>
    _parseInfixExpression(std::shared_ptr<AST::expression> leftNode);
}; // class Parser
} // namespace parser
#endif // PARSER_HPP