#ifndef FML_AST_INTERPRETER_EXECVISITOR_H
#define FML_AST_INTERPRETER_EXECVISITOR_H

#include "../AstNodes.h"

class ExecVisitor : public Visitor {
  public:
    void visit(const ast::Top* visitable) const override;
    void visit(const ast::Print* visitable) const override;
    void visit(const ast::CallMethod* visitable) const override;
    void visit(const ast::Integer* visitable) const override;
};


#endif //FML_AST_INTERPRETER_EXECVISITOR_H
