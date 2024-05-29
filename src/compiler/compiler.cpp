#include "compiler.hpp"

compiler::Compiler::Compiler() : llvm_context(llvm::LLVMContext()), llvm_ir_builder(llvm_context) {
    this->llvm_module = std::make_unique<llvm::Module>("main", llvm_context);
    this->_initializeBuiltins();
}

void compiler::Compiler::_initializeBuiltins() {
    this->type_map["int"] = llvm::Type::getInt32Ty(llvm_context);
    this->type_map["float"] = llvm::Type::getFloatTy(llvm_context);
    this->type_map["bool"] = llvm::Type::getInt1Ty(llvm_context);

    // Create the global variable 'true'
    llvm::GlobalVariable* globalTrue = new llvm::GlobalVariable(*this->llvm_module, this->type_map["bool"], true, llvm::GlobalValue::ExternalLinkage,
                                                                llvm::ConstantInt::get(this->type_map["bool"], 1), "True");

    // Create the global variable 'false'
    llvm::GlobalVariable* globalFalse = new llvm::GlobalVariable(*this->llvm_module, this->type_map["bool"], true, llvm::GlobalValue::ExternalLinkage,
                                                                 llvm::ConstantInt::get(this->type_map["bool"], 0), "False");
    this->enviornment.add("True", this->type_map["bool"], globalTrue, nullptr);
    this->enviornment.add("False", this->type_map["bool"], globalFalse, nullptr);
};

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
    case AST::NodeType::VariableAssignmentStatement: {
        this->_visitVariableAssignmentStatement(std::static_pointer_cast<AST::VariableAssignmentStatement>(node));
        break;
    }
    case AST::NodeType::IfElseStatement: {
        this->_visitIfElseStatement(std::static_pointer_cast<AST::IfElseStatement>(node));
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
    case AST::NodeType::CallExpression: {
        this->_visitCallExpression(std::static_pointer_cast<AST::CallExpression>(node));
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
        std::cout << "Unknown node type" << std::endl;
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
            result_type = this->type_map["int"];
        } else if(op == token::TokenType::Dash) {
            result_value = this->llvm_ir_builder.CreateSub(left_value, right_value, "subtmp");
            result_type = this->type_map["int"];
        } else if(op == token::TokenType::Asterisk) {
            result_value = this->llvm_ir_builder.CreateMul(left_value, right_value, "multmp");
            result_type = this->type_map["int"];
        } else if(op == token::TokenType::ForwardSlash) {
            result_value = this->llvm_ir_builder.CreateSDiv(left_value, right_value, "divtmp");
            result_type = this->type_map["int"];
        } else if(op == token::TokenType::GreaterThan) {
            result_value = this->llvm_ir_builder.CreateICmpSGT(left_value, right_value, "gttmp");
            result_type = this->type_map["bool"];
        } else if(op == token::TokenType::LessThan) {
            result_value = this->llvm_ir_builder.CreateICmpSLT(left_value, right_value, "lttmp");
            result_type = this->type_map["bool"];
        } else if(op == token::TokenType::EqualEqual) {
            result_value = this->llvm_ir_builder.CreateICmpEQ(left_value, right_value, "eqtmp");
            result_type = this->type_map["bool"];
        } else if(op == token::TokenType::GreaterThanOrEqual) {
            result_value = this->llvm_ir_builder.CreateICmpSGE(left_value, right_value, "getmp");
            result_type = this->type_map["bool"];
        } else if(op == token::TokenType::LessThanOrEqual) {
            result_value = this->llvm_ir_builder.CreateICmpSLE(left_value, right_value, "letmp");
            result_type = this->type_map["bool"];
        } else if(op == token::TokenType::NotEquals) {
            result_value = this->llvm_ir_builder.CreateICmpNE(left_value, right_value, "netmp");
            result_type = this->type_map["bool"];
        } else {
            std::cout << "Unknown operator" << std::endl;
        }
    } else if(left_type == this->type_map["float"] && right_type == this->type_map["float"]) {
        if(op == token::TokenType::Plus) {
            result_value = this->llvm_ir_builder.CreateFAdd(left_value, right_value, "addtmp");
            result_type = this->type_map["float"];
        } else if(op == token::TokenType::Dash) {
            result_value = this->llvm_ir_builder.CreateFSub(left_value, right_value, "subtmp");
            result_type = this->type_map["float"];
        } else if(op == token::TokenType::Asterisk) {
            result_value = this->llvm_ir_builder.CreateFMul(left_value, right_value, "multmp");
            result_type = this->type_map["float"];
        } else if(op == token::TokenType::ForwardSlash) {
            result_value = this->llvm_ir_builder.CreateFDiv(left_value, right_value, "divtmp");
            result_type = this->type_map["float"];
        } else if(op == token::TokenType::GreaterThan) {
            result_value = this->llvm_ir_builder.CreateFCmpOGT(left_value, right_value, "gttmp");
            result_type = this->type_map["bool"];
        } else if(op == token::TokenType::LessThan) {
            result_value = this->llvm_ir_builder.CreateFCmpOLT(left_value, right_value, "lttmp");
            result_type = this->type_map["bool"];
        } else if(op == token::TokenType::EqualEqual) {
            result_value = this->llvm_ir_builder.CreateFCmpOEQ(left_value, right_value, "eqtmp");
            result_type = this->type_map["bool"];
        } else if(op == token::TokenType::GreaterThanOrEqual) {
            result_value = this->llvm_ir_builder.CreateFCmpOGE(left_value, right_value, "getmp");
            result_type = this->type_map["bool"];
        } else if(op == token::TokenType::LessThanOrEqual) {
            result_value = this->llvm_ir_builder.CreateFCmpOLE(left_value, right_value, "letmp");
            result_type = this->type_map["bool"];
        } else if(op == token::TokenType::NotEquals) {
            result_value = this->llvm_ir_builder.CreateFCmpONE(left_value, right_value, "netmp");
            result_type = this->type_map["bool"];
        } else {
            std::cout << "Unknown operator" << std::endl;
        }
    }
    //  else if(left_type == this->type_map["bool"] && right_type == this->type_map["bool"]) {
    //     if(op == token::TokenType::And) {
    //         result_value = this->llvm_ir_builder.CreateAnd(left_value, right_value, "andtmp");
    //         result_type = this->type_map["bool"];
    //     } else if(op == token::TokenType::Or) {
    //         result_value = this->llvm_ir_builder.CreateOr(left_value, right_value, "ortmp");
    //         result_type = this->type_map["bool"];
    //     } else {
    //         std::cout << "Unknown operator" << std::endl;
    //     }
    // } else {
    //     std::cout << "Unknown operator" << std::endl;
    // }
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

void compiler::Compiler::_visitVariableAssignmentStatement(std::shared_ptr<AST::VariableAssignmentStatement> variable_assignment_statement) {
    auto var_name = std::static_pointer_cast<AST::IdentifierLiteral>(variable_assignment_statement->name);
    auto var_value = variable_assignment_statement->value;
    auto [value, type] = this->_resolveValue(var_value);
    if(this->enviornment.get(var_name->value) != std::make_tuple(nullptr, nullptr, nullptr)) {
        auto [_, __, alloca] = this->enviornment.get(var_name->value);
        this->llvm_ir_builder.CreateStore(value, alloca);
    } else {
        std::cout << "Variable not declared" << std::endl;
    }
};

void compiler::Compiler::_visitIfElseStatement(std::shared_ptr<AST::IfElseStatement> if_statement) {
    auto condition = if_statement->condition;
    auto consequence = if_statement->consequence;
    auto alternative = if_statement->alternative;
    auto [condition_val, _] = this->_resolveValue(condition);
    if(alternative == nullptr) {
        auto func = this->llvm_ir_builder.GetInsertBlock()->getParent();
        llvm::BasicBlock* ThenBB = llvm::BasicBlock::Create(llvm_context, "then", func);
        llvm::BasicBlock* ContBB = llvm::BasicBlock::Create(llvm_context, "cont", func);
        this->llvm_ir_builder.CreateCondBr(condition_val, ThenBB, ContBB);
        this->llvm_ir_builder.SetInsertPoint(ThenBB);
        this->compile(consequence);
        this->llvm_ir_builder.CreateBr(ContBB);
        this->llvm_ir_builder.SetInsertPoint(ContBB);
    } else {
        auto func = this->llvm_ir_builder.GetInsertBlock()->getParent();
        llvm::BasicBlock* ThenBB = llvm::BasicBlock::Create(llvm_context, "then", func);
        llvm::BasicBlock* ElseBB = llvm::BasicBlock::Create(llvm_context, "else", func);
        llvm::BasicBlock* ContBB = llvm::BasicBlock::Create(llvm_context, "cont", func);
        this->llvm_ir_builder.CreateCondBr(condition_val, ThenBB, ElseBB);
        this->llvm_ir_builder.SetInsertPoint(ThenBB);
        this->compile(consequence);
        this->llvm_ir_builder.CreateBr(ContBB);
        this->llvm_ir_builder.SetInsertPoint(ElseBB);
        this->compile(alternative);
        this->llvm_ir_builder.CreateBr(ContBB);
        this->llvm_ir_builder.SetInsertPoint(ContBB);
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
    std::vector<std::string> param_name;
    std::vector<llvm::Type*> param_types;
    for(auto param : params) {
        param_name.push_back(std::static_pointer_cast<AST::IdentifierLiteral>(param->name)->value);
        param_types.push_back(
            this->type_map[std::dynamic_pointer_cast<AST::IdentifierLiteral>(std::dynamic_pointer_cast<AST::GenericType>(param->value_type)->name)
                               ->value]);
    }
    auto return_type = this->type_map[std::dynamic_pointer_cast<AST::IdentifierLiteral>(
                                          std::dynamic_pointer_cast<AST::GenericType>(function_declaration_statement->returnType)->name)
                                          ->value];
    auto func_type = llvm::FunctionType::get(return_type, param_types, false);
    auto func = llvm::Function::Create(func_type, llvm::Function::ExternalLinkage, name, this->llvm_module.get());

    // name the parameters
    unsigned idx = 0;
    for(auto& arg : func->args()) {
        arg.setName(param_name[idx++]);
    }
    auto bb = llvm::BasicBlock::Create(llvm_context, name + "_entry", func);
    this->llvm_ir_builder.SetInsertPoint(bb);

    auto prev_env = std::make_shared<enviornment::Enviornment>(this->enviornment);
    this->enviornment = enviornment::Enviornment(prev_env, {}, name);
    this->enviornment.add(name, func->getFunctionType(), func, nullptr);
    // adding the alloca for the parameters
    for(auto& arg : func->args()) {
        auto alloca = this->llvm_ir_builder.CreateAlloca(arg.getType(), nullptr, arg.getName());
        this->llvm_ir_builder.CreateStore(&arg, alloca);
        this->enviornment.add(std::string(arg.getName()), arg.getType(), &arg, alloca);
    }
    this->current_function = func;
    this->compile(body);
    this->enviornment = *prev_env;
    this->enviornment.add(name, func->getFunctionType(), func, nullptr);
}

std::tuple<llvm::Value*, llvm::Type*> compiler::Compiler::_visitCallExpression(std::shared_ptr<AST::CallExpression> call_expression) {
    auto name = std::static_pointer_cast<AST::IdentifierLiteral>(call_expression->name)->value;
    auto param = call_expression->arguments;
    auto args = std::vector<llvm::Value*>();
    for(auto arg : param) {
        auto [value, type] = this->_resolveValue(arg);
        args.push_back(value);
    }
    if(this->enviornment.get(name) != std::make_tuple(nullptr, nullptr, nullptr)) {
        auto [func, func_type, _] = this->enviornment.get(name);
        auto returnValue = this->llvm_ir_builder.CreateCall(llvm::cast<llvm::Function>(func), args, "calltmp");
        return {returnValue, func_type};
    }
    std::cout << "Function not declared" << std::endl;
    return {nullptr, nullptr};
};

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
        auto x = std::tuple<llvm::Value*, llvm::Type*>{this->llvm_ir_builder.CreateLoad(type, alloca, identifier_literal->value), type};
        return x;
    }
    case AST::NodeType::CallExpression: {
        auto call_expression = std::static_pointer_cast<AST::CallExpression>(node);
        return this->_visitCallExpression(call_expression);
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
        std::cout << "Unknown node type" << std::endl;
        return {nullptr, nullptr};
    }
}
