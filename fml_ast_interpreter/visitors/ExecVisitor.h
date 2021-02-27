#ifndef FML_AST_INTERPRETER_EXECVISITOR_H
#define FML_AST_INTERPRETER_EXECVISITOR_H

#include "../AstNodes.h"

class ExecVisitor : public Visitor {
  public:
    void visit(const ast::Top* visitable) const override;
    void visit(const ast::Print* visitable) const override;
    void visit(const ast::CallMethod* visitable) const override;
    void visit(const ast::Integer* visitable) const override;
    void visit(const ast::Boolean* visitable) const override;
    void visit(const ast::Null* visitable) const override;
    void visit(const ast::Variable* visitable) const override;
    void visit(const ast::AccessVariable* visitable) const override;
    void visit(const ast::AssignVariable* visitable) const override;
    void visit(const ast::Function* visitable) const override;
    void visit(const ast::CallFunction* visitable) const override;
    void visit(const ast::Block* visitable) const override;
};


#endif //FML_AST_INTERPRETER_EXECVISITOR_H
