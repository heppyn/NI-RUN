#ifndef FML_AST_INTERPRETER_ASTNODES_H
#define FML_AST_INTERPRETER_ASTNODES_H

#include <memory>
#include <vector>

class Visitor;

namespace ast {
struct AST {
    virtual ~AST() = default;
    virtual std::unique_ptr<AST> accept(Visitor*) const = 0;
    [[nodiscard]] virtual std::string toString() const { return "AST"; }
    [[nodiscard]] virtual std::unique_ptr<AST> clone() const = 0;
};
template<typename Derived>
struct VisitableAST : AST {
    std::unique_ptr<AST> accept(Visitor* visitor) const override;
};

struct Integer : public VisitableAST<Integer> {
    Integer() = default;
    [[nodiscard]] std::unique_ptr<AST> clone() const override;
    explicit Integer(int i) : value(i) {}
    [[nodiscard]] std::string toString() const override {
        return std::to_string(value);
    }
    int value{};
};
struct Boolean : public VisitableAST<Boolean> {
    Boolean() = default;
    explicit Boolean(bool val) : value(val) {}
    [[nodiscard]] std::unique_ptr<AST> clone() const override;
    [[nodiscard]] std::string toString() const override {
        return value ? "true" : "false";
    }
    bool value{};
};
struct Null : public VisitableAST<Null> {
    [[nodiscard]] std::unique_ptr<AST> clone() const override;
    [[nodiscard]] std::string toString() const override {
        return "null";
    }
};
struct Variable : public VisitableAST<Variable> {
    Variable() = default;
    Variable(std::string name, std::unique_ptr<AST> value);
    [[nodiscard]] std::unique_ptr<AST> clone() const override;
    std::string name{};
    std::unique_ptr<AST> value{};
};
struct AccessVariable : public VisitableAST<AccessVariable> {
    AccessVariable() = default;
    AccessVariable(std::string name);
    [[nodiscard]] std::unique_ptr<AST> clone() const override;
    std::string name{};
};
struct AssignVariable : public VisitableAST<AssignVariable> {
    AssignVariable() = default;
    AssignVariable(std::string name, std::unique_ptr<AST> value);
    [[nodiscard]] std::unique_ptr<AST> clone() const override;
    std::string name{};
    std::unique_ptr<AST> value{};
};
struct Function : public VisitableAST<Function> {
    Function() = default;
    Function(std::string name, std::vector<std::string> parameters, std::unique_ptr<AST> body);
    [[nodiscard]] std::unique_ptr<AST> clone() const override;
    std::string name{};
    std::vector<std::string> parameters{};
    std::unique_ptr<AST> body{};
};
struct CallFunction : public VisitableAST<CallFunction> {
    CallFunction() = default;
    CallFunction(std::string name, const std::vector<std::unique_ptr<AST>>& args);
    [[nodiscard]] std::unique_ptr<AST> clone() const override;
    std::string name{};
    std::vector<std::unique_ptr<AST>> arguments{};
};
struct CallMethod : public VisitableAST<CallMethod> {
    CallMethod() = default;
    CallMethod(std::unique_ptr<AST> object, std::string name, const std::vector<std::unique_ptr<AST>>& args);
    [[nodiscard]] std::unique_ptr<AST> clone() const override;
    std::unique_ptr<AST> object{ nullptr };
    std::string name{};
    std::vector<std::unique_ptr<AST>> arguments{};
};
struct Print : public VisitableAST<Print> {
    Print() = default;
    Print(std::string format, const std::vector<std::unique_ptr<AST>>& args);
    [[nodiscard]] std::unique_ptr<AST> clone() const override;
    std::string format{};
    std::vector<std::unique_ptr<AST>> arguments{};
};
struct Block : public VisitableAST<Block> {
    Block() = default;
    explicit Block(const std::vector<std::unique_ptr<AST>>& stms);
    [[nodiscard]] std::unique_ptr<AST> clone() const override;
    std::vector<std::unique_ptr<AST>> stms{};
};
struct Top : public VisitableAST<Top> {
    Top() = default;
    explicit Top(const std::vector<std::unique_ptr<AST>>& stms);
    [[nodiscard]] std::unique_ptr<AST> clone() const override;
    std::vector<std::unique_ptr<AST>> stms{};
};
//    struct Loop : public VisitableAST<Loop> {
//        std::unique_ptr<AST> condition{};
//        std::unique_ptr<AST> body{};
//    };
//    struct Conditional : public VisitableAST<Conditional> {
//        std::unique_ptr<AST> condition{};
//        std::unique_ptr<AST> consequent{};
//        std::unique_ptr<AST> alternative{};
//    };
} // namespace ast

class Visitor {
  public:
    virtual std::unique_ptr<ast::AST> visit(const ast::Top* visitable) = 0;
    virtual std::unique_ptr<ast::AST> visit(const ast::Print* visitable) = 0;
    virtual std::unique_ptr<ast::AST> visit(const ast::CallMethod* visitable) = 0;
    virtual std::unique_ptr<ast::AST> visit(const ast::Integer* visitable) = 0;
    virtual std::unique_ptr<ast::AST> visit(const ast::Boolean* visitable) = 0;
    virtual std::unique_ptr<ast::AST> visit(const ast::Null* visitable) = 0;
    virtual std::unique_ptr<ast::AST> visit(const ast::Variable* visitable) = 0;
    virtual std::unique_ptr<ast::AST> visit(const ast::AccessVariable* visitable) = 0;
    virtual std::unique_ptr<ast::AST> visit(const ast::AssignVariable* visitable) = 0;
    virtual std::unique_ptr<ast::AST> visit(const ast::Function* visitable) = 0;
    virtual std::unique_ptr<ast::AST> visit(const ast::CallFunction* visitable) = 0;
    virtual std::unique_ptr<ast::AST> visit(const ast::Block* visitable) = 0;
};

namespace ast {
template<typename Derived>
std::unique_ptr<AST> VisitableAST<Derived>::accept(Visitor* visitor) const {
    return visitor->visit(static_cast<const Derived*>(this));
}
} // namespace ast

#endif //FML_AST_INTERPRETER_ASTNODES_H
