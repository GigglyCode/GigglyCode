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

    static const std::unordered_map<token::TokenType, PrecedenceType> token_precedence = {
        {token::TokenType::Illegal, PrecedenceType::LOWEST},
        {token::TokenType::GreaterThan, PrecedenceType::COMPARISION},
        {token::TokenType::LessThan, PrecedenceType::COMPARISION},
        {token::TokenType::GreaterThanOrEqual, PrecedenceType::COMPARISION},
        {token::TokenType::LessThanOrEqual, PrecedenceType::COMPARISION},
        {token::TokenType::EqualEqual, PrecedenceType::COMPARISION},
        {token::TokenType::NotEquals, PrecedenceType::COMPARISION},
        {token::TokenType::PlusEqual, PrecedenceType::ASSIGN},
        {token::TokenType::DashEqual, PrecedenceType::ASSIGN},
        {token::TokenType::AsteriskEqual, PrecedenceType::ASSIGN},
        {token::TokenType::PercentEqual, PrecedenceType::ASSIGN},
        {token::TokenType::CaretEqual, PrecedenceType::ASSIGN},
        {token::TokenType::ForwardSlashEqual, PrecedenceType::ASSIGN},
        {token::TokenType::BackwardSlashEqual, PrecedenceType::ASSIGN},
        {token::TokenType::Equals, PrecedenceType::ASSIGN},
        {token::TokenType::Is, PrecedenceType::ASSIGN},
        {token::TokenType::Increment, PrecedenceType::POSTFIX},
        {token::TokenType::Decrement, PrecedenceType::POSTFIX},
        {token::TokenType::BitwiseAnd, PrecedenceType::COMPARISION},
        {token::TokenType::BitwiseOr, PrecedenceType::COMPARISION},
        {token::TokenType::BitwiseXor, PrecedenceType::COMPARISION},
        {token::TokenType::BitwiseNot, PrecedenceType::PREFIX},
        {token::TokenType::LeftShift, PrecedenceType::COMPARISION},
        {token::TokenType::RightShift, PrecedenceType::COMPARISION},
        {token::TokenType::Dot, PrecedenceType::MEMBER_ACCESS},
        {token::TokenType::Ellipsis, PrecedenceType::LOWEST},
        {token::TokenType::Plus, PrecedenceType::SUM},
        {token::TokenType::Dash, PrecedenceType::SUM},
        {token::TokenType::Asterisk, PrecedenceType::PRODUCT},
        {token::TokenType::Percent, PrecedenceType::PRODUCT},
        {token::TokenType::AsteriskAsterisk, PrecedenceType::Exponent},
        {token::TokenType::ForwardSlash, PrecedenceType::PRODUCT},
        {token::TokenType::BackwardSlash, PrecedenceType::PRODUCT},
        {token::TokenType::EndOfFile, PrecedenceType::LOWEST},
    };

    class Parser {
      public:
        std::shared_ptr<Lexer> lexer;
        std::shared_ptr<token::Token> current_token;
        std::shared_ptr<token::Token> peek_token;
        std::vector<std::shared_ptr<errors::Error>> errors;
        std::unordered_map<token::TokenType, std::function<std::shared_ptr<AST::Expression>()>> prefix_parse_fns = {
            {token::TokenType::Integer, std::bind(&Parser::_parseIntegerLiteral, this)},
            {token::TokenType::Float, std::bind(&Parser::_parseFloatLiteral, this)},
            {token::TokenType::String, std::bind(&Parser::_parseStringLiteral, this)},
            {token::TokenType::True, std::bind(&Parser::_parseBooleanLiteral, this)},
            {token::TokenType::False, std::bind(&Parser::_parseBooleanLiteral, this)},
            {token::TokenType::Identifier, std::bind(&Parser::_parseIdentifier, this)},
            {token::TokenType::LeftParen, std::bind(&Parser::_parseGroupedExpression, this)},
        };
        std::unordered_map<token::TokenType, std::function<std::shared_ptr<AST::Expression>(std::shared_ptr<AST::Expression>)>> infix_parse_Fns = {
            {token::TokenType::Plus, std::bind(&Parser::_parseInfixExpression, this, std::placeholders::_1)},
            {token::TokenType::Dash, std::bind(&Parser::_parseInfixExpression, this, std::placeholders::_1)},
            {token::TokenType::Asterisk, std::bind(&Parser::_parseInfixExpression, this, std::placeholders::_1)},
            {token::TokenType::ForwardSlash, std::bind(&Parser::_parseInfixExpression, this, std::placeholders::_1)},
            {token::TokenType::Percent, std::bind(&Parser::_parseInfixExpression, this, std::placeholders::_1)},
            {token::TokenType::AsteriskAsterisk, std::bind(&Parser::_parseInfixExpression, this, std::placeholders::_1)},
        };
        Parser(std::shared_ptr<Lexer> lexer);
        std::shared_ptr<AST::Program> parseProgram();

      private:
        void _nextToken();
        bool _currentTokenIs(token::TokenType type);
        bool _peekTokenIs(token::TokenType type);
        bool _expectPeek(token::TokenType type);
        void _peekError(token::TokenType type, token::TokenType expected_type, std::string suggestedFix = "");
        void _noPrefixParseFnError(token::TokenType type);
        PrecedenceType _currentPrecedence();
        PrecedenceType _peekPrecedence();
        std::shared_ptr<AST::Statement> _parseStatement();

        std::shared_ptr<AST::ExpressionStatement> _parseExpressionStatement();
        std::shared_ptr<AST::Statement> _parseVariableDeclaration();
        std::shared_ptr<AST::Statement> _parseVariableAssignment();
        std::shared_ptr<AST::ReturnStatement> _parseReturnStatement();
        std::shared_ptr<AST::FunctionStatement> _parseFunctionStatement();
        std::shared_ptr<AST::BlockStatement> _parseBlockStatement();

        std::shared_ptr<AST::BaseType> _parseType();

        std::shared_ptr<AST::Expression> _parseExpression(PrecedenceType precedence);

        std::shared_ptr<AST::Expression> _parseIntegerLiteral();
        std::shared_ptr<AST::Expression> _parseFloatLiteral();
        std::shared_ptr<AST::Expression> _parseBooleanLiteral();
        std::shared_ptr<AST::Expression> _parseStringLiteral();
        std::shared_ptr<AST::Expression> _parseGroupedExpression();
        std::shared_ptr<AST::Expression> _parseIdentifier();

        std::shared_ptr<AST::Expression> _parseInfixExpression(std::shared_ptr<AST::Expression> leftNode);
    }; // class Parser
} // namespace parser
#endif // PARSER_HPP