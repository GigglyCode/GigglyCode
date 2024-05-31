#ifndef AST_HPP
#define AST_HPP
#include "../../include/json.hpp"
#include "../../lexer/tokens.hpp"
#include <tuple>
#include <unordered_map>


namespace AST {
enum class NodeType {
    Program,
    Unknown,

    // Statements
    ExpressionStatement,
    VariableDeclarationStatement,
    VariableAssignmentStatement,
    FunctionStatement,
    FunctionParameter,
    CallExpression,
    BlockStatement,
    ReturnStatement,
    IfElseStatement,

    // Types
    Type,
    UnionType,

    // Expressions
    InfixedExpression,

    // Literals
    IntegerLiteral,
    FloatLiteral,
    BooleanLiteral,
    StringLiteral,
    IdentifierLiteral,
};

std::shared_ptr<std::string> nodeTypeToString(NodeType type);

struct MetaData {
    int st_line_no = -1;
    int st_col_no = -1;
    int end_line_no = -1;
    int end_col_no = -1;
    NodeType type = NodeType::Unknown;
    std::unordered_map<std::string, std::tuple<int, int>> more_data = {};
};

class Node {
  public:
    MetaData meta_data;
    virtual inline void set_meta_data(int st_line_no, int st_col_no, int end_line_no, int end_col_no) {
        meta_data.st_line_no = st_line_no;
        meta_data.st_col_no = st_col_no;
        meta_data.end_line_no = meta_data.end_line_no;
        meta_data.end_col_no = meta_data.end_col_no;
    };
    virtual inline NodeType type() { return NodeType::Unknown; };
    virtual inline std::shared_ptr<nlohmann::json> toJSON() {
        auto json = nlohmann::json();
        json["type"] = *nodeTypeToString(this->type());
        return std::make_shared<nlohmann::json>(json);
    };
};

class Statement : public Node {};

class Expression : public Node {};

class BaseType : public Node {};

class GenericType : public BaseType {
  public:
    std::shared_ptr<Expression> name;
    std::vector<std::shared_ptr<BaseType>> generics;
    inline GenericType(std::shared_ptr<Expression> name, std::vector<std::shared_ptr<BaseType>> generics) : name(name), generics(generics) {}
    inline NodeType type() override { return NodeType::Type; };
    std::shared_ptr<nlohmann::json> toJSON() override;
};

class Program : public Node {
  public:
    std::vector<std::shared_ptr<Statement>> statements;
    inline NodeType type() override { return NodeType::Program; };
    std::shared_ptr<nlohmann::json> toJSON() override;
};

class ExpressionStatement : public Statement {
  public:
    std::shared_ptr<Expression> expr;
    inline ExpressionStatement(std::shared_ptr<Expression> expr = nullptr) : expr(expr) {}
    inline NodeType type() override { return NodeType::ExpressionStatement; };
    std::shared_ptr<nlohmann::json> toJSON() override;
};

class BlockStatement : public Statement {
  public:
    std::vector<std::shared_ptr<Statement>> statements;
    inline NodeType type() override { return NodeType::BlockStatement; };
    inline BlockStatement(std::vector<std::shared_ptr<Statement>> statements = {}) : statements(statements) {}
    std::shared_ptr<nlohmann::json> toJSON() override;
};

class ReturnStatement : public Statement {
  public:
    std::shared_ptr<Expression> value;
    inline ReturnStatement(std::shared_ptr<Expression> value = nullptr) : value(value) {}
    inline NodeType type() override { return NodeType::ReturnStatement; };
    std::shared_ptr<nlohmann::json> toJSON() override;
};

class FunctionParameter : public Node {
  public:
    std::shared_ptr<Expression> name;
    std::shared_ptr<BaseType> value_type;
    inline FunctionParameter(std::shared_ptr<Expression> name, std::shared_ptr<BaseType> type) : name(name), value_type(type) {}
    inline NodeType type() override { return NodeType::FunctionParameter; };
    std::shared_ptr<nlohmann::json> toJSON() override;
};

class FunctionStatement : public Statement {
  public:
    std::shared_ptr<Expression> name;
    std::vector<std::shared_ptr<FunctionParameter>> parameters;
    std::shared_ptr<BaseType> returnType;
    std::shared_ptr<BlockStatement> body;
    inline FunctionStatement(std::shared_ptr<Expression> name, std::vector<std::shared_ptr<FunctionParameter>> parameters,
                             std::shared_ptr<BaseType> returnType, std::shared_ptr<BlockStatement> body)
        : name(name), parameters(parameters), returnType(returnType), body(body) {}
    inline NodeType type() override { return NodeType::FunctionStatement; };
    std::shared_ptr<nlohmann::json> toJSON() override;
};

class CallExpression : public Expression {
  public:
    std::shared_ptr<Expression> name;
    std::vector<std::shared_ptr<Expression>> arguments;
    inline CallExpression(std::shared_ptr<Expression> name, std::vector<std::shared_ptr<Expression>> arguments = {})
        : name(name), arguments(arguments) {}
    inline NodeType type() override { return NodeType::CallExpression; };
    std::shared_ptr<nlohmann::json> toJSON() override;
};

class IfElseStatement : public Statement {
  public:
    std::shared_ptr<Expression> condition;
    std::shared_ptr<Statement> consequence;
    std::shared_ptr<Statement> alternative;
    inline IfElseStatement(std::shared_ptr<Expression> condition, std::shared_ptr<Statement> consequence,
                           std::shared_ptr<Statement> alternative = nullptr)
        : condition(condition), consequence(consequence), alternative(alternative) {}
    inline NodeType type() override { return NodeType::IfElseStatement; };
    std::shared_ptr<nlohmann::json> toJSON() override;
};

class VariableDeclarationStatement : public Statement {
  public:
    std::shared_ptr<Expression> name;
    std::shared_ptr<BaseType> value_type;
    std::shared_ptr<Expression> value;
    inline VariableDeclarationStatement(std::shared_ptr<Expression> name, std::shared_ptr<BaseType> type, std::shared_ptr<Expression> value = nullptr)
        : name(name), value_type(type), value(value) {}
    inline NodeType type() override { return NodeType::VariableDeclarationStatement; };
    std::shared_ptr<nlohmann::json> toJSON() override;
};

class VariableAssignmentStatement : public Statement {
  public:
    std::shared_ptr<Expression> name;
    std::shared_ptr<Expression> value;
    inline VariableAssignmentStatement(std::shared_ptr<Expression> name, std::shared_ptr<Expression> value) : name(name), value(value) {}
    inline NodeType type() override { return NodeType::VariableAssignmentStatement; };
    std::shared_ptr<nlohmann::json> toJSON() override;
};

class InfixExpression : public Expression {
  public:
    std::shared_ptr<Expression> left;
    std::shared_ptr<Expression> right;
    token::TokenType op;
    inline InfixExpression(std::shared_ptr<Expression> left, token::TokenType op, std::shared_ptr<Expression> right = nullptr)
        : left(left), right(right), op(op) {}
    inline NodeType type() override { return NodeType::InfixedExpression; };
    std::shared_ptr<nlohmann::json> toJSON() override;
};

class IntegerLiteral : public Expression {
  public:
    long long int value;
    inline IntegerLiteral(long long int value) : value(value) {}
    inline NodeType type() override { return NodeType::IntegerLiteral; };
    std::shared_ptr<nlohmann::json> toJSON() override;
};

class FloatLiteral : public Expression {
  public:
    double value;
    inline FloatLiteral(double value) : value(value) {}
    inline NodeType type() override { return NodeType::FloatLiteral; };
    std::shared_ptr<nlohmann::json> toJSON() override;
};

class StringLiteral : public Expression {
  public:
    std::string value;
    inline StringLiteral(std::string value) : value(value) { this->meta_data.more_data["length"] = {value.length(), value.length()}; }
    inline NodeType type() override { return NodeType::StringLiteral; };
    std::shared_ptr<nlohmann::json> toJSON() override;
};

class IdentifierLiteral : public Expression {
  public:
    std::string value;
    inline IdentifierLiteral(std::string value) : value(value) {}
    inline NodeType type() override { return NodeType::IdentifierLiteral; };
    std::shared_ptr<nlohmann::json> toJSON() override;
};

class BooleanLiteral : public Expression {
  public:
    bool value;
    inline BooleanLiteral(bool value) : value(value) {}
    inline NodeType type() override { return NodeType::BooleanLiteral; };
    std::shared_ptr<nlohmann::json> toJSON() override;
};
} // namespace AST
#endif // AST_HPP