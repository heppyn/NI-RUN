#ifndef FML_AST_INTERPRETER_ASTNODES_H
#define FML_AST_INTERPRETER_ASTNODES_H

#include <memory>
#include <vector>

class Visitor;

namespace ast {
struct AST {
    virtual ~AST() = default;
    virtual std::unique_ptr<AST> accept(const Visitor*) const = 0;
    virtual std::string toString() const { return "AST"; }
};
template<typename Derived>
struct VisitableAST : AST {
    std::unique_ptr<AST> accept(const Visitor* visitor) const override;
};

struct Integer : public VisitableAST<Integer> {
    Integer() = default;
    explicit Integer(int i) : value(i) {}
    [[nodiscard]] std::string toString() const override {
        return std::to_string(value);
    }
    int value{};
};
struct Boolean : public VisitableAST<Boolean> {
    Boolean() = default;
    explicit Boolean(bool val) : value(val) {}
    [[nodiscard]] std::string toString() const override {
        return value ? "true" : "false";
    }
    bool value{};
};
struct Null : public VisitableAST<Null> {
    [[nodiscard]] std::string toString() const override {
        return "null";
    }
};
struct Variable : public VisitableAST<Variable> {
    std::string name{};
    std::unique_ptr<AST> value{};
};
struct AccessVariable : public VisitableAST<AccessVariable> {
    std::string name{};
};
struct AssignVariable : public VisitableAST<AssignVariable> {
    std::string name{};
    std::unique_ptr<AST> value{};
};
struct Function : public VisitableAST<Function> {
    std::string name{};
    std::vector<std::string> parameters{};
    std::unique_ptr<AST> body{};
};
struct CallFunction : public VisitableAST<CallFunction> {
    std::string name{};
    std::vector<std::unique_ptr<AST>> arguments{};
};
struct CallMethod : public VisitableAST<CallMethod> {
    std::unique_ptr<AST> object{ nullptr };
    std::string name{};
    std::vector<std::unique_ptr<AST>> arguments{};
};
struct Print : public VisitableAST<Print> {
    std::string format{};
    std::vector<std::unique_ptr<AST>> arguments{};
};
struct Block : public VisitableAST<Block> {
    std::vector<std::unique_ptr<AST>> stms{};
};
struct Top : public VisitableAST<Top> {
    std::vector<std::unique_ptr<AST>> stms;
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
    virtual std::unique_ptr<ast::AST> visit(const ast::Top* visitable) const = 0;
    virtual std::unique_ptr<ast::AST> visit(const ast::Print* visitable) const = 0;
    virtual std::unique_ptr<ast::AST> visit(const ast::CallMethod* visitable) const = 0;
    virtual std::unique_ptr<ast::AST> visit(const ast::Integer* visitable) const = 0;
    virtual std::unique_ptr<ast::AST> visit(const ast::Boolean* visitable) const = 0;
    virtual std::unique_ptr<ast::AST> visit(const ast::Null* visitable) const = 0;
    virtual std::unique_ptr<ast::AST> visit(const ast::Variable* visitable) const = 0;
    virtual std::unique_ptr<ast::AST> visit(const ast::AccessVariable* visitable) const = 0;
    virtual std::unique_ptr<ast::AST> visit(const ast::AssignVariable* visitable) const = 0;
    virtual std::unique_ptr<ast::AST> visit(const ast::Function* visitable) const = 0;
    virtual std::unique_ptr<ast::AST> visit(const ast::CallFunction* visitable) const = 0;
    virtual std::unique_ptr<ast::AST> visit(const ast::Block* visitable) const = 0;
};

namespace ast {
template<typename Derived>
std::unique_ptr<AST> VisitableAST<Derived>::accept(const Visitor* visitor) const {
    return visitor->visit(static_cast<const Derived*>(this));
}
} // namespace ast

#endif //FML_AST_INTERPRETER_ASTNODES_H
