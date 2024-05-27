#include <llvm/IR/Instructions.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <memory>
#include <string>
#include <tuple>
#include <unordered_map>


namespace enviornment {
class Enviornment {
    using EnviornmentMap = std::unordered_map<std::string, std::tuple<llvm::Value*, llvm::Type*, llvm::AllocaInst*>>;

  public:
    std::shared_ptr<Enviornment> parent;
    std::string name;
    EnviornmentMap records;
    Enviornment(std::shared_ptr<Enviornment> parent = nullptr, EnviornmentMap records = {}, std::string name = "unnamed")
        : parent(parent), name(name), records(records) {};
    void add(std::string name, llvm::Type* type, llvm::Value* value, llvm::AllocaInst* alloca);
    std::tuple<llvm::Value*, llvm::Type*, llvm::AllocaInst*> get(std::string name);
}; // class Environment
} // namespace enviornment