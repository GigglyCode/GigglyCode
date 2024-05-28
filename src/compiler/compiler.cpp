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
        break;
    }
    case AST::NodeType::BlockStatement: {
        this->_visitBlockStatement(std::static_pointer_cast<AST::BlockStatement>(node));
        break;
    }
    case AST::NodeType::FunctionStatement: {
        this->_visitFunctionDeclarationStatement(std::static_pointer_cast<AST::FunctionStatement>(node));
        break;
    }
    case AST::NodeType::ReturnStatement: {
        this->_visitReturnStatement(std::static_pointer_cast<AST::ReturnStatement>(node));
        break;
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
    // std::string entry_point_name = "main";
    // auto param_types = std::vector<llvm::Type*>();
    // auto return_type = this->type_map["int"];
    // auto func_type = llvm::FunctionType::get(return_type, param_types, false);
    // auto func = llvm::Function::Create(func_type, llvm::Function::ExternalLinkage, entry_point_name, this->llvm_module.get());
    // auto bb = llvm::BasicBlock::Create(llvm_context, "entry", func);
    // this->llvm_ir_builder.SetInsertPoint(bb);
    for(auto stmt : program->statements) {
        this->compile(stmt);
    }
    // this->llvm_ir_builder.CreateRet(llvm::ConstantInt::get(llvm_context, llvm::APInt(32, 0, true)));
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

void compiler::Compiler::_visitBlockStatement(std::shared_ptr<AST::BlockStatement> block_statement) {
    for(auto stmt : block_statement->statements) {
        this->compile(stmt);
    }
};

void compiler::Compiler::_visitFunctionDeclarationStatement(std::shared_ptr<AST::FunctionStatement> function_declaration_statement) {
    auto name = std::static_pointer_cast<AST::IdentifierLiteral>(function_declaration_statement->name)->value;
    auto body = function_declaration_statement->body;
    auto params = function_declaration_statement->parameters;
    auto param_types = std::vector<llvm::Type*>();
    auto param_names = std::vector<std::string>();
    for(auto param : params) {
        param_names.push_back(std::static_pointer_cast<AST::IdentifierLiteral>(param->name)->value);
        param_types.push_back(
            this->type_map[std::dynamic_pointer_cast<AST::IdentifierLiteral>(std::dynamic_pointer_cast<AST::GenericType>(param->value_type)->name)
                               ->value]);
    }
    auto return_type = this->type_map[std::dynamic_pointer_cast<AST::IdentifierLiteral>(
                                          std::dynamic_pointer_cast<AST::GenericType>(function_declaration_statement->returnType)->name)
                                          ->value];
    auto func_type = llvm::FunctionType::get(return_type, param_types, false);
    auto func = llvm::Function::Create(func_type, llvm::Function::ExternalLinkage, name, this->llvm_module.get());
    auto bb = llvm::BasicBlock::Create(llvm_context, name + "_entry", func);
    this->llvm_ir_builder.SetInsertPoint(bb);
    // ignore the parameters for now
    auto prev_env = std::make_shared<enviornment::Enviornment>(this->enviornment);
    this->enviornment = enviornment::Enviornment(prev_env, {}, name);
    this->enviornment.add(name, func->getFunctionType(), func, nullptr);
    this->compile(body);
    this->enviornment = *prev_env;
    this->enviornment.add(name, func->getFunctionType(), func, nullptr);
}

void compiler::Compiler::_visitReturnStatement(std::shared_ptr<AST::ReturnStatement> return_statement) {
    auto value = return_statement->value;
    auto [return_value, return_type] = this->_resolveValue(value);
    this->llvm_ir_builder.CreateRet(return_value);
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
        std::cout << "it is not fucked\n";
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
