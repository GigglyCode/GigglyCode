#include "include/AST.hpp"
#include "AST.hpp"

std::shared_ptr<nlohmann::json> AST::Node::toJson()
{
    std::shared_ptr<nlohmann::json> json = std::make_shared<nlohmann::json>();
    json->operator[]("type") = NodeTypeToString(this->type());
    return json;
}

AST::NodeType AST::Program::type()
{
    return NodeType::Program;
}

std::shared_ptr<nlohmann::json> AST::Program::toJson()
{
    std::shared_ptr<nlohmann::json> json = std::make_shared<nlohmann::json>();
    json->operator[]("type") = NodeTypeToString(this->type());
    std::shared_ptr<nlohmann::json> statements = std::make_shared<nlohmann::json>(nlohmann::json::array());
    for (auto statement : this->statements)
    {
        std::shared_ptr<nlohmann::json> stmt = std::make_shared<nlohmann::json>();
        stmt->operator[](NodeTypeToString(statement->type())) = *(statement->toJson());
        statements->push_back(*stmt);
    }
    json->operator[]("statements") = *statements;
    return json;
};

std::shared_ptr<nlohmann::json> AST::ExpressionStatement::toJson()
{
    std::shared_ptr<nlohmann::json> json = std::make_shared<nlohmann::json>();
    json->operator[]("type") = NodeTypeToString(this->type());
    json->operator[]("expression") = *(this->expression->toJson());
    return json;
}

std::shared_ptr<nlohmann::json> AST::InfixExpression::toJson()
{
    std::shared_ptr<nlohmann::json> json = std::make_shared<nlohmann::json>();
    json->operator[]("type") = NodeTypeToString(this->type());
    json->operator[]("operator") = this->operator_;
    json->operator[]("left_node") = *(this->left->toJson());
    json->operator[]("right_node") = *(this->right->toJson());
    return json;
}

std::shared_ptr<nlohmann::json> AST::IntegerLiteral::toJson()
{
    std::shared_ptr<nlohmann::json> json = std::make_shared<nlohmann::json>();
    json->operator[]("type") = NodeTypeToString(this->type());
    json->operator[]("value") = this->value;
    return json;
}

std::shared_ptr<nlohmann::json> AST::FloatLiteral::toJson()
{
    std::shared_ptr<nlohmann::json> json = std::make_shared<nlohmann::json>();
    json->operator[]("type") = NodeTypeToString(this->type());
    json->operator[]("value") = this->value;
    return json;
}

std::string AST::NodeTypeToString(NodeType type)
{
    switch (type)
    {
    case AST::NodeType::Program:
        return std::string("Program");
    case AST::NodeType::ExpressionStatement:
        return std::string("ExpressionStatement");
    case AST::NodeType::InfixExpression:
        return std::string("InfixExpression");
    case AST::NodeType::IntegerLiteral:
        return std::string("IntegerLiteral");
    case AST::NodeType::FloatLiteral:
        return std::string("FloatLiteral");
    case AST::NodeType::Unknown:
        return std::string("Unknown");
    default:
        return std::string("Unknown");
    }
};