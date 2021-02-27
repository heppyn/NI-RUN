#include "AstNodes.h"

#include <memory>

namespace ast {
std::unique_ptr<AST> ast::Integer::clone() const {
    return std::make_unique<Integer>(value);
}
std::unique_ptr<AST> Boolean::clone() const {
    return std::make_unique<Boolean>(value);
}
std::unique_ptr<AST> Null::clone() const {
    return std::make_unique<Null>();
}
std::unique_ptr<AST> Variable::clone() const {
    return std::make_unique<Variable>(name, value->clone());
}
Variable::Variable(std::string name, std::unique_ptr<AST> value)
  : name(std::move(name)), value(std::move(value)) {
}
std::unique_ptr<AST> AccessVariable::clone() const {
    return std::make_unique<AccessVariable>(name);
}
AccessVariable::AccessVariable(std::string name)
  : name(std::move(name)) {
}
std::unique_ptr<AST> AssignVariable::clone() const {
    return std::make_unique<AssignVariable>(name, value->clone());
}
AssignVariable::AssignVariable(std::string name, std::unique_ptr<AST> value)
  : name(std::move(name)), value(std::move(value)) {
}
std::unique_ptr<AST> Function::clone() const {
    return std::make_unique<Function>(name, parameters, body->clone());
}
Function::Function(std::string name, std::vector<std::string> parameters, std::unique_ptr<AST> body)
  : name(std::move(name)), parameters(std::move(parameters)), body(std::move(body)) {
}
std::unique_ptr<AST> CallFunction::clone() const {
    return std::make_unique<CallFunction>(name, arguments);
}
CallFunction::CallFunction(std::string name, const std::vector<std::unique_ptr<AST>>& args)
  : name(std::move(name)), arguments() {
    for (const auto& a : args) {
        arguments.emplace_back(a->clone());
    }
}
std::unique_ptr<AST> CallMethod::clone() const {
    return std::make_unique<CallMethod>(object->clone(), name, arguments);
}
CallMethod::CallMethod(std::unique_ptr<AST> object, std::string name, const std::vector<std::unique_ptr<AST>>& args)
  : object(std::move(object)), name(std::move(name)) {
    for (const auto& a : args) {
        arguments.emplace_back(a->clone());
    }
}
std::unique_ptr<AST> Print::clone() const {
    return std::make_unique<Print>(format, arguments);
}
Print::Print(std::string format, const std::vector<std::unique_ptr<AST>>& args)
  : format(std::move(format)) {
    for (const auto& a : args) {
        arguments.emplace_back(a->clone());
    }
}
std::unique_ptr<AST> Block::clone() const {
    return std::make_unique<Block>(stms);
}
Block::Block(const std::vector<std::unique_ptr<AST>>& stms) {
    for (const auto& a : stms) {
        this->stms.emplace_back(a->clone());
    }
}
std::unique_ptr<AST> Top::clone() const {
    return std::make_unique<Top>(stms);
}
Top::Top(const std::vector<std::unique_ptr<AST>>& stms) {
    for (const auto& a : stms) {
        this->stms.emplace_back(a->clone());
    }
}
} // namespace ast
