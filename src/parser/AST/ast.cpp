#include "./ast.hpp"

std::shared_ptr<std::string> AST::nodeTypeToString(nodeType type)
{
    switch (type)
    {
    case nodeType::Program:
        return std::make_shared<std::string>("Program");
    case nodeType::ExpressionStatement:
        return std::make_shared<std::string>("ExpressionStatement");
    case nodeType::VariableDeclarationStatement:
        return std::make_shared<std::string>("VariableDeclarationStatement");
    case nodeType::VariableAssignmentStatement:
        return std::make_shared<std::string>("VariableAssignmentStatement");
    case nodeType::TypeStatement:
        return std::make_shared<std::string>("TypeStatement");
    case nodeType::InfixedExpression:
        return std::make_shared<std::string>("InfixedExpression");
    case nodeType::IntegerLiteral:
        return std::make_shared<std::string>("IntegerLiteral");
    case nodeType::FloatLiteral:
        return std::make_shared<std::string>("FloatLiteral");
    case nodeType::StringLiteral:
        return std::make_shared<std::string>("StringLiteral");
    case nodeType::IdentifierLiteral:
        return std::make_shared<std::string>("IdentifierLiteral");
    default:
        return std::make_shared<std::string>("UNKNOWN");
    }
};
std::shared_ptr<nlohmann::json> AST::program::toJSON()
{
    auto jsonAst = nlohmann::json();
    jsonAst["type"] = *nodeTypeToString(this->type());
    jsonAst["statements"] = nlohmann::json::array();
    for (auto &stmt : this->statements)
    {
        jsonAst["statements"].push_back(*stmt->toJSON());
    }
    return std::make_shared<nlohmann::json>(jsonAst);
}

std::shared_ptr<nlohmann::json> AST::expressionStatement::toJSON()
{
    auto jsonAst = nlohmann::json();
    jsonAst["type"] = *nodeTypeToString(this->type());
    jsonAst["expression"] = this->expr == nullptr ? nullptr : *expr->toJSON();
    return std::make_shared<nlohmann::json>(jsonAst);
}

std::shared_ptr<nlohmann::json> AST::variableDeclarationStatement::toJSON()
{
    auto jsonAst = nlohmann::json();
    jsonAst["type"] = *nodeTypeToString(this->type());
    jsonAst["identifier"] = *this->name->toJSON();
    jsonAst["value_type"] = *this->valueType->toJSON();
    jsonAst["value"] = this->value == nullptr ? nullptr : *value->toJSON();
    return std::make_shared<nlohmann::json>(jsonAst);
}

std::shared_ptr<nlohmann::json> AST::variableAssignmentStatement::toJSON()
{
    auto jsonAst = nlohmann::json();
    jsonAst["type"] = *nodeTypeToString(this->type());
    jsonAst["identifier"] = *this->name->toJSON();
    jsonAst["value"] = *value->toJSON();
    return std::make_shared<nlohmann::json>(jsonAst);
}

std::shared_ptr<nlohmann::json> AST::infixExpression::toJSON()
{
    auto jsonAst = nlohmann::json();
    jsonAst["type"] = *nodeTypeToString(this->type());
    jsonAst["operator"] = *token::tokenTypeString(this->op);
    jsonAst["left_node"] = *(left->toJSON());
    jsonAst["right_node"] = this->right == nullptr ? nullptr : *right->toJSON();
    return std::make_shared<nlohmann::json>(jsonAst);
}

std::shared_ptr<nlohmann::json> AST::integerLiteral::toJSON()
{
    auto jsonAst = nlohmann::json();
    jsonAst["type"] = *nodeTypeToString(this->type());
    jsonAst["value"] = this->value;
    return std::make_shared<nlohmann::json>(jsonAst);
}

std::shared_ptr<nlohmann::json> AST::floatLiteral::toJSON()
{
    auto jsonAst = nlohmann::json();
    jsonAst["type"] = *nodeTypeToString(this->type());
    jsonAst["value"] = this->value;
    return std::make_shared<nlohmann::json>(jsonAst);
}

std::shared_ptr<nlohmann::json> AST::identifierLiteral::toJSON()
{
    auto jsonAst = nlohmann::json();
    jsonAst["type"] = *nodeTypeToString(this->type());
    jsonAst["value"] = this->value;
    return std::make_shared<nlohmann::json>(jsonAst);
}