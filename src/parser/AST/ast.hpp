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
        VariableDeclarationStatement,
        VariableAssignmentStatement,
        TypeStatement,

        // Expressions
        InfixedExpression,

        // Literals
        IntegerLiteral,
        FloatLiteral,
        StringLiteral,
        IdentifierLiteral,
    };

    std::shared_ptr<std::string> nodeTypeToString(nodeType type);

    class node
    {
    public:
        virtual inline nodeType type() { return nodeType::Unknown; };
        virtual inline std::shared_ptr<nlohmann::json> toJSON()
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
        inline nodeType type() override { return nodeType::Program; };
        std::shared_ptr<nlohmann::json> toJSON() override;
    };

    class expressionStatement : public statement
    {
    public:
        std::shared_ptr<expression> expr;
        inline expressionStatement(std::shared_ptr<expression> expr = nullptr) : expr(expr) {}
        inline nodeType type() override { return nodeType::ExpressionStatement; };
        std::shared_ptr<nlohmann::json> toJSON() override;
    };

    class variableDeclarationStatement : public statement
    {
    public:
        std::shared_ptr<expression> name;
        std::shared_ptr<expression> valueType;
        std::shared_ptr<expression> value;
        inline variableDeclarationStatement(std::shared_ptr<expression> name, std::shared_ptr<expression> type, std::shared_ptr<expression> value = nullptr) : name(name), valueType(type), value(value) {}
        inline nodeType type() override { return nodeType::VariableDeclarationStatement; };
        std::shared_ptr<nlohmann::json> toJSON() override;
    };

    class variableAssignmentStatement : public statement
    {
    public:
        std::shared_ptr<expression> name;
        std::shared_ptr<expression> value;
        inline variableAssignmentStatement(std::shared_ptr<expression> name, std::shared_ptr<expression> value) : name(name), value(value) {}
        inline nodeType type() override { return nodeType::VariableAssignmentStatement; };
        std::shared_ptr<nlohmann::json> toJSON() override;
    };

    // class typeStatement : public statement
    // {
    // public:
    //     std::shared_ptr<expression> name;
    //     std::shared_ptr<expression> value;
    //     inline typeStatement(std::shared_ptr<expression> name, std::shared_ptr<expression> value) : name(name), value(value) {}
    //     inline nodeType type() override { return nodeType::TypeStatement; };
    //     std::shared_ptr<nlohmann::json> toJSON() override;
    // };

    class infixExpression : public expression
    {
    public:
        std::shared_ptr<expression> left;
        std::shared_ptr<expression> right;
        token::tokenType op;
        inline infixExpression(std::shared_ptr<expression> left, token::tokenType op, std::shared_ptr<expression> right = nullptr) : left(left), right(right), op(op) {}
        inline nodeType type() override { return nodeType::InfixedExpression; };
        std::shared_ptr<nlohmann::json> toJSON() override;
    };

    class integerLiteral : public expression
    {
    public:
        long long int value;
        inline integerLiteral(long long int value) : value(value) {}
        inline nodeType type() override { return nodeType::IntegerLiteral; };
        std::shared_ptr<nlohmann::json> toJSON() override;
    };

    class floatLiteral : public expression
    {
    public:
        double value;
        inline floatLiteral(double value) : value(value) {}
        inline nodeType type() override { return nodeType::FloatLiteral; };
        std::shared_ptr<nlohmann::json> toJSON() override;
    };

    class identifierLiteral : public expression
    {
    public:
        std::string value;
        inline identifierLiteral(std::string value) : value(value) {}
        inline nodeType type() override { return nodeType::IdentifierLiteral; };
        std::shared_ptr<nlohmann::json> toJSON() override;
    };
}
#endif // AST_HPP