#include "enviornment.hpp"

void enviornment::Enviornment::add(std::string name, llvm::Type* type, llvm::Value* value, llvm::AllocaInst* alloca) {
    records[name] = std::make_tuple(value, type, alloca);
}
std::tuple<llvm::Value*, llvm::Type*, llvm::AllocaInst*> enviornment::Enviornment::get(std::string name) {
    if(records.find(name) != records.end()) {
        return records[name];
    } else if(parent != nullptr) {
        return parent->get(name);
    } else {
        return std::make_tuple(nullptr, nullptr, nullptr);
    }
}