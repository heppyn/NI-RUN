#ifndef FML_AST_INTERPRETER_ASTNODES_H
#define FML_AST_INTERPRETER_ASTNODES_H

#include <memory>
#include <vector>

class Visitor;

namespace ast {
struct AST {
    virtual ~AST() = default;
    virtual void accept(const Visitor*) const = 0;
};
template<typename Derived>
struct VisitableAST : AST {
    void accept(const Visitor* visitor) const override;
};

struct Integer : public VisitableAST<Integer> {
    int value{};
};
struct Boolean : public VisitableAST<Boolean> {
    bool value{};
};
struct Null : public VisitableAST<Null> {
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
    virtual void visit(const ast::Top* visitable) const = 0;
    virtual void visit(const ast::Print* visitable) const = 0;
    virtual void visit(const ast::CallMethod* visitable) const = 0;
    virtual void visit(const ast::Integer* visitable) const = 0;
    virtual void visit(const ast::Boolean* visitable) const = 0;
    virtual void visit(const ast::Null* visitable) const = 0;
    virtual void visit(const ast::Variable* visitable) const = 0;
    virtual void visit(const ast::AccessVariable* visitable) const = 0;
    virtual void visit(const ast::AssignVariable* visitable) const = 0;
    virtual void visit(const ast::Function* visitable) const = 0;
    virtual void visit(const ast::CallFunction* visitable) const = 0;
    virtual void visit(const ast::Block* visitable) const = 0;
};

namespace ast {
template<typename Derived>
void VisitableAST<Derived>::accept(const Visitor* visitor) const {
    visitor->visit(static_cast<const Derived*>(this));
}
} // namespace ast

#endif //FML_AST_INTERPRETER_ASTNODES_H
