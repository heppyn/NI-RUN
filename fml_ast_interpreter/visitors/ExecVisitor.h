#ifndef FML_AST_INTERPRETER_EXECVISITOR_H
#define FML_AST_INTERPRETER_EXECVISITOR_H

#include "../AstNodes.h"
#include "../Environment.h"

class ExecVisitor : public Visitor {
  private:
    Environment* m_env = new Environment{ nullptr };
    Environment* m_env_prev = nullptr;

  public:
    ~ExecVisitor() {
        delete m_env;
        delete m_env_prev;
    }

    std::unique_ptr<ast::AST> visit(const ast::Top* visitable) override;
    std::unique_ptr<ast::AST> visit(const ast::Print* visitable) override;
    std::unique_ptr<ast::AST> visit(const ast::CallMethod* visitable) override;
    std::unique_ptr<ast::AST> visit(const ast::Integer* visitable) override;
    std::unique_ptr<ast::AST> visit(const ast::Boolean* visitable) override;
    std::unique_ptr<ast::AST> visit(const ast::Null* visitable) override;
    std::unique_ptr<ast::AST> visit(const ast::Variable* visitable) override;
    std::unique_ptr<ast::AST> visit(const ast::AccessVariable* visitable) override;
    std::unique_ptr<ast::AST> visit(const ast::AssignVariable* visitable) override;
    std::unique_ptr<ast::AST> visit(const ast::Function* visitable) override;
    std::unique_ptr<ast::AST> visit(const ast::CallFunction* visitable) override;
    std::unique_ptr<ast::AST> visit(const ast::Block* visitable) override;
    std::unique_ptr<ast::AST> visit(const ast::Loop* visitable) override;
    std::unique_ptr<ast::AST> visit(const ast::Conditional* visitable) override;

  private:
    std::unique_ptr<ast::AST> evaluate(const ast::AST* stm);
    void beginScope();
    void endScope();
};


#endif //FML_AST_INTERPRETER_EXECVISITOR_H
