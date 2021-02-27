#ifndef FML_AST_INTERPRETER_EXECVISITOR_H
#define FML_AST_INTERPRETER_EXECVISITOR_H

#include "../AstNodes.h"

class ExecVisitor : public Visitor {
  public:
    std::unique_ptr<ast::AST> visit(const ast::Top* visitable) const override;
    std::unique_ptr<ast::AST> visit(const ast::Print* visitable) const override;
    std::unique_ptr<ast::AST> visit(const ast::CallMethod* visitable) const override;
    std::unique_ptr<ast::AST> visit(const ast::Integer* visitable) const override;
    std::unique_ptr<ast::AST> visit(const ast::Boolean* visitable) const override;
    std::unique_ptr<ast::AST> visit(const ast::Null* visitable) const override;
    std::unique_ptr<ast::AST> visit(const ast::Variable* visitable) const override;
    std::unique_ptr<ast::AST> visit(const ast::AccessVariable* visitable) const override;
    std::unique_ptr<ast::AST> visit(const ast::AssignVariable* visitable) const override;
    std::unique_ptr<ast::AST> visit(const ast::Function* visitable) const override;
    std::unique_ptr<ast::AST> visit(const ast::CallFunction* visitable) const override;
    std::unique_ptr<ast::AST> visit(const ast::Block* visitable) const override;

  private:
    std::unique_ptr<ast::AST> evaluate(const ast::AST* stm) const;
};


#endif //FML_AST_INTERPRETER_EXECVISITOR_H
