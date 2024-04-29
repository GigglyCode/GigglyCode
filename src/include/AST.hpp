#ifndef AST_HPP
#define AST_HPP
#include <string>
#include "json.hpp"

namespace AST
{
    enum class NodeType
    {
        Program,

        ExpressionStatement,
        VariableDeclarationStatement,

        InfixExpression,

        IntegerLiteral,
        FloatLiteral,
        IdentifierLiteral,

        Unknown

    };

    std::string NodeTypeToString(NodeType type);

    class Node
    {
    public:
        inline virtual NodeType type() { return AST::NodeType::Unknown; };
        virtual std::shared_ptr<nlohmann::json> toJson();
    };

    class Statement : public Node
    {
    };

    class Expression : public Node
    {
    };

    class Program : public Node
    {
    public:
        std::vector<std::shared_ptr<Statement>> statements;
        NodeType type() override;
        std::shared_ptr<nlohmann::json> toJson() override;
    };

    class IntegerLiteral : public Expression
    {
    public:
        int value;
        IntegerLiteral(int val) : value(val){};
        NodeType type() override { return NodeType::IntegerLiteral; };
        std::shared_ptr<nlohmann::json> toJson() override;
    };

    class FloatLiteral : public Expression
    {
    public:
        float value;
        FloatLiteral(float val) : value(val){};
        NodeType type() override { return NodeType::FloatLiteral; };
        std::shared_ptr<nlohmann::json> toJson() override;
    };

    class IdentifierLiteral : public Expression
    {
    public:
        std::string value;
        IdentifierLiteral(std::string val) : value(val){};
        NodeType type() override { return NodeType::IdentifierLiteral; };
        std::shared_ptr<nlohmann::json> toJson() override;
    };

    class ExpressionStatement : public Statement
    {
    public:
        std::shared_ptr<Expression> expression;
        ExpressionStatement(std::shared_ptr<Expression> expr)
            : expression(expr){};
        NodeType type() override { return NodeType::ExpressionStatement; };
        std::shared_ptr<nlohmann::json> toJson() override;
    };

    class VariableDeclarationStatement : public Statement
    {
    public:
        std::shared_ptr<IdentifierLiteral> name = nullptr;
        std::shared_ptr<Expression> value = nullptr;
        std::shared_ptr<IdentifierLiteral> valueType = nullptr;
        VariableDeclarationStatement(std::shared_ptr<IdentifierLiteral> n, std::shared_ptr<Expression> val, std::shared_ptr<IdentifierLiteral> vt = nullptr)
            : name(n), value(val), valueType(vt){};
        VariableDeclarationStatement(){};
        NodeType type() override { return NodeType::VariableDeclarationStatement; };
        std::shared_ptr<nlohmann::json> toJson() override;
    };

    class InfixExpression : public Expression
    {
    public:
        std::shared_ptr<Expression> left;
        std::shared_ptr<Expression> right;
        std::string operator_;
        InfixExpression(std::shared_ptr<Expression> l, std::string op, std::shared_ptr<Expression> r = nullptr)
            : left(l), right(r), operator_(op){};
        NodeType type() override { return NodeType::InfixExpression; };
        std::shared_ptr<nlohmann::json> toJson() override;
    };
}
#endif