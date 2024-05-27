#include "compiler.hpp"

compiler::Compiler::Compiler() : llvm_context(llvm::LLVMContext()), llvm_ir_builder(llvm_context) {
    this->llvm_module = std::make_unique<llvm::Module>("main", llvm_context);
    this->type_map["int"] = llvm::Type::getInt32Ty(llvm_context);
    this->type_map["float"] = llvm::Type::getFloatTy(llvm_context);
    this->type_map["bool"] = llvm::Type::getInt1Ty(llvm_context);
}

void compiler::Compiler::compile(std::shared_ptr<AST::Node> node) {
    switch(node->type()) {
    case AST::NodeType::Program:
        this->_visitProgram(std::static_pointer_cast<AST::Program>(node));
        break;
    case AST::NodeType::ExpressionStatement: {
        this->_visitExpressionStatement(std::static_pointer_cast<AST::ExpressionStatement>(node));
        break;
    }
    case AST::NodeType::InfixedExpression: {
        this->_visitInfixExpression(std::static_pointer_cast<AST::InfixExpression>(node));
        break;
    }
    case AST::NodeType::VariableDeclarationStatement: {
        this->_visitVariableDeclarationStatement(std::static_pointer_cast<AST::VariableDeclarationStatement>(node));
    }
    case AST::NodeType::BooleanLiteral: {
        auto boolean_literal = std::static_pointer_cast<AST::BooleanLiteral>(node);
        auto value = llvm::ConstantInt::get(llvm_context, llvm::APInt(1, boolean_literal->value, true));
        break;
    }
    default:
        break;
    }
};

void compiler::Compiler::_visitProgram(std::shared_ptr<AST::Program> program) {
    std::string entry_point_name = "main";
    auto param_types = std::vector<llvm::Type*>();
    auto return_type = this->type_map["int"];
    auto func_type = llvm::FunctionType::get(return_type, param_types, false);
    auto func = llvm::Function::Create(func_type, llvm::Function::ExternalLinkage, entry_point_name, this->llvm_module.get());
    auto bb = llvm::BasicBlock::Create(llvm_context, "entry", func);
    this->llvm_ir_builder.SetInsertPoint(bb);
    for(auto stmt : program->statements) {
        this->compile(stmt);
    }
    this->llvm_ir_builder.CreateRet(llvm::ConstantInt::get(llvm_context, llvm::APInt(32, 0, true)));
};

void compiler::Compiler::_visitExpressionStatement(std::shared_ptr<AST::ExpressionStatement> expression_statement) {
    this->compile(expression_statement->expr);
};

std::tuple<llvm::Value*, llvm::Type*> compiler::Compiler::_visitInfixExpression(std::shared_ptr<AST::InfixExpression> infixed_expression) {
    auto op = infixed_expression->op;
    auto left = infixed_expression->left;
    auto right = infixed_expression->right;
    auto [left_value, left_type] = this->_resolveValue(left);
    auto [right_value, right_type] = this->_resolveValue(right);
    llvm::Value* result_value = nullptr;
    llvm::Type* result_type = nullptr;
    if(left_type == this->type_map["int"] && right_type == this->type_map["int"]) {
        if(op == token::TokenType::Plus) {
            result_value = this->llvm_ir_builder.CreateAdd(left_value, right_value, "addtmp");
        } else if(op == token::TokenType::Dash) {
            result_value = this->llvm_ir_builder.CreateSub(left_value, right_value, "subtmp");
        } else if(op == token::TokenType::Asterisk) {
            result_value = this->llvm_ir_builder.CreateMul(left_value, right_value, "multmp");
        } else if(op == token::TokenType::ForwardSlash) {
            result_value = this->llvm_ir_builder.CreateSDiv(left_value, right_value, "divtmp");
        } else {
            std::cout << "Unknown operator" << std::endl;
        }
        result_type = this->type_map["int"];
    } else if(left_type == this->type_map["float"] && right_type == this->type_map["float"]) {
        if(op == token::TokenType::Plus) {
            result_value = this->llvm_ir_builder.CreateFAdd(left_value, right_value, "addtmp");
        } else if(op == token::TokenType::Dash) {
            result_value = this->llvm_ir_builder.CreateFSub(left_value, right_value, "subtmp");
        } else if(op == token::TokenType::Asterisk) {
            result_value = this->llvm_ir_builder.CreateFMul(left_value, right_value, "multmp");
        } else if(op == token::TokenType::ForwardSlash) {
            result_value = this->llvm_ir_builder.CreateFDiv(left_value, right_value, "divtmp");
        }
        result_type = this->type_map["float"];
    }
    return {result_value, result_type};
};

void compiler::Compiler::_visitVariableDeclarationStatement(std::shared_ptr<AST::VariableDeclarationStatement> variable_declaration_statement) {
    auto var_name = std::static_pointer_cast<AST::IdentifierLiteral>(variable_declaration_statement->name);
    auto var_value = variable_declaration_statement->value;
    auto var_type = this->type_map[std::dynamic_pointer_cast<AST::IdentifierLiteral>(
                                       std::dynamic_pointer_cast<AST::GenericType>(variable_declaration_statement->value_type)->name)
                                       ->value];
    auto [value, type] = this->_resolveValue(var_value);
    if(this->enviornment.get(var_name->value) == std::make_tuple(nullptr, nullptr, nullptr)) {
        llvm::AllocaInst* alloca =
            this->llvm_ir_builder.CreateAlloca(var_type, nullptr, std::dynamic_pointer_cast<AST::IdentifierLiteral>(var_name)->value);
        this->llvm_ir_builder.CreateStore(value, alloca);
        this->enviornment.add(var_name->value, var_type, value, alloca);
    } else {
        std::cout << "Variable already declared" << std::endl;
    }
};

std::tuple<llvm::Value*, llvm::Type*> compiler::Compiler::_resolveValue(std::shared_ptr<AST::Node> node) {
    switch(node->type()) {
    case AST::NodeType::IntegerLiteral: {
        auto integer_literal = std::static_pointer_cast<AST::IntegerLiteral>(node);
        auto value = llvm::ConstantInt::get(llvm_context, llvm::APInt(32, integer_literal->value, true));
        return {value, this->type_map["int"]};
    }
    case AST::NodeType::FloatLiteral: {
        auto float_literal = std::static_pointer_cast<AST::FloatLiteral>(node);
        auto value = llvm::ConstantFP::get(llvm_context, llvm::APFloat(float_literal->value));
        return {value, this->type_map["float"]};
    }
    case AST::NodeType::BooleanLiteral: {
        auto boolean_literal = std::static_pointer_cast<AST::BooleanLiteral>(node);
        auto value = llvm::ConstantInt::get(llvm_context, llvm::APInt(1, boolean_literal->value, true));
        return {value, this->type_map["bool"]};
    }
    case AST::NodeType::IdentifierLiteral: {
        auto identifier_literal = std::static_pointer_cast<AST::IdentifierLiteral>(node);
        auto [value, type, alloca] = this->enviornment.get(identifier_literal->value);
        std::cout << "some thing is going to fuck\n";
        auto x = std::tuple<llvm::Value*, llvm::Type*>{this->llvm_ir_builder.CreateLoad(type, alloca, identifier_literal->value), type};
        std::cout << "it is not fucked";
        return x;
    }
    case AST::NodeType::ExpressionStatement: {
        auto expression_statement = std::static_pointer_cast<AST::ExpressionStatement>(node);
        return this->_resolveValue(expression_statement->expr);
    }
    case AST::NodeType::InfixedExpression: {
        auto infix_expression = std::static_pointer_cast<AST::InfixExpression>(node);
        return this->_visitInfixExpression(infix_expression);
    }
    default:
        return {nullptr, nullptr};
    }
}
