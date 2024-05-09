#ifndef AST_HPP
#define AST_HPP
#include "../../lexer/tokens.hpp"
#include "../../include/json.hpp"

namespace AST
{
    enum class nodeType
    {
        Program,
        Unknown,

        // Statements
        ExpressionStatement,

        // Expressions
        InfixedExpression,

        // Literals
        IntegerLiteral,
        FloatLiteral,
        StringLiteral,
    };

    std::shared_ptr<std::string> nodeTypeToString(nodeType type);

    class node
    {
    public:
        virtual nodeType type() { return nodeType::Unknown; };
        virtual std::shared_ptr<nlohmann::json> toJSON()
        {
            auto json = nlohmann::json();
            json["type"] = *nodeTypeToString(this->type());
            return std::make_shared<nlohmann::json>(json);
        };
    };

    class statement : public node
    {
    };

    class expression : public node
    {
    };

    class program : public node
    {
    public:
        std::vector<std::shared_ptr<statement>> statements;
        nodeType type() override { return nodeType::Program; };
        std::shared_ptr<nlohmann::json> toJSON() override;
    };

    class expressionStatement : public statement
    {
    public:
        std::shared_ptr<expression> expr;
        expressionStatement(std::shared_ptr<expression> expr = nullptr) : expr(expr) {}
        nodeType type() override { return nodeType::ExpressionStatement; };
        std::shared_ptr<nlohmann::json> toJSON() override;
    };

    class infixExpression : public expression
    {
    public:
        std::shared_ptr<expression> left;
        std::shared_ptr<expression> right;
        token::tokenType op;
        infixExpression(std::shared_ptr<expression> left, token::tokenType op, std::shared_ptr<expression> right = nullptr) : left(left), right(right), op(op) {}
        nodeType type() override { return nodeType::InfixedExpression; };
        std::shared_ptr<nlohmann::json> toJSON() override;
    };

    class integerLiteral : public expression
    {
    public:
        long long int value;
        integerLiteral(long long int value) : value(value) {}
        nodeType type() override { return nodeType::IntegerLiteral; };
        std::shared_ptr<nlohmann::json> toJSON() override;
    };

    class floatLiteral : public expression
    {
    public:
        double value;
        floatLiteral(double value) : value(value) {}
        nodeType type() override { return nodeType::FloatLiteral; };
        std::shared_ptr<nlohmann::json> toJSON() override;
    };
}
#endif // AST_HPP