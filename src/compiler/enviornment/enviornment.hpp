#include <llvm/IR/Instructions.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <memory>
#include <string>
#include <tuple>
#include <unordered_map>


namespace enviornment {
enum class RecordType { RecordClassType, RecordEnumType, RecordVariable, RecordFunction };
class Record {
  public:
    RecordType type;
    std::string name;
    Record(RecordType type, std::string name) : type(type), name(name) {};
}; // class Record

class RecordClassType : public Record {
  public:
    RecordClassType(std::string name) : Record(RecordType::RecordClassType, name) {};
};

class RecordEnumType : public Record {
  public:
    RecordEnumType(std::string name) : Record(RecordType::RecordEnumType, name) {};
};

class RecordVariable : public Record {
  public:
    llvm::Value* value;
    llvm::Type* type;
    llvm::AllocaInst* allocainst;
    RecordVariable(std::string name) : Record(RecordType::RecordVariable, name) {};
    RecordVariable(std::string name, llvm::Value* value, llvm::Type* type, llvm::AllocaInst* allocainst)
        : Record(RecordType::RecordVariable, name), value(value), type(type), allocainst(allocainst) {};
};

class RecordFunction : public Record {
  public:
    llvm::Function* function;
    llvm::FunctionType* function_type;
    std::vector<std::tuple<std::string, std::shared_ptr<RecordVariable>>> arguments;
    RecordFunction(std::string name) : Record(RecordType::RecordFunction, name) {};
    RecordFunction(std::string name, llvm::Function* function, llvm::FunctionType* function_type,
                   std::vector<std::tuple<std::string, std::shared_ptr<RecordVariable>>> arguments)
        : Record(RecordType::RecordFunction, name), function(function), function_type(function_type), arguments(arguments) {};
};

class Enviornment {
  public:
    std::shared_ptr<Enviornment> parent;
    std::string name;
    std::unordered_map<std::string, std::shared_ptr<Record>> record_map;
    Enviornment(std::shared_ptr<Enviornment> parent = nullptr, std::unordered_map<std::string, std::shared_ptr<Record>> records = {},
                std::string name = "unnamed")
        : parent(parent), name(name), record_map(records) {};
    void add(std::shared_ptr<Record> record);
    std::shared_ptr<Record> get(std::string name);
    bool contains(std::string name);
    bool is_variable(std::string name);
    std::shared_ptr<RecordVariable> get_variable(std::string name);
    bool is_function(std::string name);
    std::shared_ptr<RecordFunction> get_function(std::string name);
    bool is_class(std::string name);
    std::shared_ptr<RecordClassType> get_class(std::string name);
    bool is_enum(std::string name);
    std::shared_ptr<RecordEnumType> get_enum(std::string name);
}; // class Environment
} // namespace enviornment