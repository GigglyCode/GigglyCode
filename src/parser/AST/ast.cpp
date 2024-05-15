#include "./ast.hpp"

std::shared_ptr<std::string> AST::nodeTypeToString(NodeType type) {
    switch(type) {
    case NodeType::Program:
        return std::make_shared<std::string>("Program");
    case NodeType::ExpressionStatement:
        return std::make_shared<std::string>("ExpressionStatement");
    case NodeType::VariableDeclarationStatement:
        return std::make_shared<std::string>("VariableDeclarationStatement");
    case NodeType::VariableAssignmentStatement:
        return std::make_shared<std::string>("VariableAssignmentStatement");
    case NodeType::FunctionStatement:
        return std::make_shared<std::string>("FunctionStatement");
    case NodeType::BlockStatement:
        return std::make_shared<std::string>("BlockStatement");
    case NodeType::ReturnStatement:
        return std::make_shared<std::string>("ReturnStatement");
    case NodeType::Type:
        return std::make_shared<std::string>("TypeStatement");
    case NodeType::UnionType:
        return std::make_shared<std::string>("UnionType");
    case NodeType::InfixedExpression:
        return std::make_shared<std::string>("InfixedExpression");
    case NodeType::IntegerLiteral:
        return std::make_shared<std::string>("IntegerLiteral");
    case NodeType::FloatLiteral:
        return std::make_shared<std::string>("FloatLiteral");
    case NodeType::StringLiteral:
        return std::make_shared<std::string>("StringLiteral");
    case NodeType::IdentifierLiteral:
        return std::make_shared<std::string>("IdentifierLiteral");
    case NodeType::BooleanLiteral:
        return std::make_shared<std::string>("BooleanLiteral");
    default:
        return std::make_shared<std::string>("UNKNOWN");
    }
};

std::shared_ptr<nlohmann::json> AST::GenericType::toJSON() {
    auto jsonAst = nlohmann::json();
    jsonAst["type"] = *nodeTypeToString(this->type());
    jsonAst["name"] = *this->name->toJSON();
    jsonAst["generics"] = nlohmann::json::array();
    for(auto& gen : this->generics) {
        jsonAst["generics"].push_back(*gen->toJSON());
    }
    return std::make_shared<nlohmann::json>(jsonAst);
};

std::shared_ptr<nlohmann::json> AST::Program::toJSON() {
    auto jsonAst = nlohmann::json();
    jsonAst["type"] = *nodeTypeToString(this->type());
    jsonAst["statements"] = nlohmann::json::array();
    for(auto& stmt : this->statements) {
        jsonAst["statements"].push_back(*stmt->toJSON());
    }
    return std::make_shared<nlohmann::json>(jsonAst);
}

std::shared_ptr<nlohmann::json> AST::ExpressionStatement::toJSON() {
    auto jsonAst = nlohmann::json();
    jsonAst["type"] = *nodeTypeToString(this->type());
    jsonAst["Expression"] = this->expr == nullptr ? nullptr : *expr->toJSON();
    return std::make_shared<nlohmann::json>(jsonAst);
}

std::shared_ptr<nlohmann::json> AST::BlockStatement::toJSON() {
    auto jsonAst = nlohmann::json();
    jsonAst["type"] = *nodeTypeToString(this->type());
    jsonAst["statements"] = nlohmann::json::array();
    for(auto& stmt : this->statements) {
        jsonAst["statements"].push_back(*stmt->toJSON());
    }
    return std::make_shared<nlohmann::json>(jsonAst);
}

std::shared_ptr<nlohmann::json> AST::ReturnStatement::toJSON() {
    auto jsonAst = nlohmann::json();
    jsonAst["type"] = *nodeTypeToString(this->type());
    jsonAst["value"] = this->value == nullptr ? nullptr : *(this->value->toJSON());
    return std::make_shared<nlohmann::json>(jsonAst);
}

std::shared_ptr<nlohmann::json> AST::FunctionStatement::toJSON() {
    auto jsonAst = nlohmann::json();
    jsonAst["type"] = *nodeTypeToString(this->type());
    jsonAst["name"] = *this->name->toJSON();
    jsonAst["parameters"] = nlohmann::json::array();
    for(auto& param : this->parameters) {
        jsonAst["parameters"].push_back(*param->toJSON());
    }
    jsonAst["return_type"] = *this->returnType->toJSON();
    jsonAst["body"] = *this->body->toJSON();
    return std::make_shared<nlohmann::json>(jsonAst);
}

std::shared_ptr<nlohmann::json> AST::FunctionStatement::parameter::toJSON() {
    auto jsonAst = nlohmann::json();
    jsonAst["type"] = *nodeTypeToString(this->type());
    jsonAst["name"] = *this->name->toJSON();
    jsonAst["type"] = *this->value_type->toJSON();
    return std::make_shared<nlohmann::json>(jsonAst);
}

std::shared_ptr<nlohmann::json> AST::VariableDeclarationStatement::toJSON() {
    auto jsonAst = nlohmann::json();
    jsonAst["type"] = *nodeTypeToString(this->type());
    jsonAst["identifier"] = *this->name->toJSON();
    jsonAst["value_type"] = *this->value_type->toJSON();
    jsonAst["value"] = this->value == nullptr ? nullptr : *value->toJSON();
    return std::make_shared<nlohmann::json>(jsonAst);
}

std::shared_ptr<nlohmann::json> AST::VariableAssignmentStatement::toJSON() {
    auto jsonAst = nlohmann::json();
    jsonAst["type"] = *nodeTypeToString(this->type());
    jsonAst["identifier"] = *this->name->toJSON();
    jsonAst["value"] = *value->toJSON();
    return std::make_shared<nlohmann::json>(jsonAst);
}

std::shared_ptr<nlohmann::json> AST::InfixExpression::toJSON() {
    auto jsonAst = nlohmann::json();
    jsonAst["type"] = *nodeTypeToString(this->type());
    jsonAst["operator"] = *token::tokenTypeString(this->op);
    jsonAst["left_node"] = *(left->toJSON());
    jsonAst["right_node"] = this->right == nullptr ? nullptr : *right->toJSON();
    return std::make_shared<nlohmann::json>(jsonAst);
}

std::shared_ptr<nlohmann::json> AST::IntegerLiteral::toJSON() {
    auto jsonAst = nlohmann::json();
    jsonAst["type"] = *nodeTypeToString(this->type());
    jsonAst["value"] = this->value;
    return std::make_shared<nlohmann::json>(jsonAst);
}

std::shared_ptr<nlohmann::json> AST::FloatLiteral::toJSON() {
    auto jsonAst = nlohmann::json();
    jsonAst["type"] = *nodeTypeToString(this->type());
    jsonAst["value"] = this->value;
    return std::make_shared<nlohmann::json>(jsonAst);
}

std::shared_ptr<nlohmann::json> AST::StringLiteral::toJSON() {
    auto jsonAst = nlohmann::json();
    jsonAst["type"] = *nodeTypeToString(this->type());
    jsonAst["value"] = this->value;
    return std::make_shared<nlohmann::json>(jsonAst);
}

std::shared_ptr<nlohmann::json> AST::IdentifierLiteral::toJSON() {
    auto jsonAst = nlohmann::json();
    jsonAst["type"] = *nodeTypeToString(this->type());
    jsonAst["value"] = this->value;
    return std::make_shared<nlohmann::json>(jsonAst);
}

std::shared_ptr<nlohmann::json> AST::BooleanLiteral::toJSON() {
    auto jsonAst = nlohmann::json();
    jsonAst["type"] = *nodeTypeToString(this->type());
    jsonAst["value"] = this->value;
    return std::make_shared<nlohmann::json>(jsonAst);
}