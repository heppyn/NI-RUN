#include "ExecVisitor.h"
#include <iostream>

void ExecVisitor::visit(const ast::Top* v) const {
    std::cout << "Visiting Top" << std::endl;
    std::cout << "Size of stms: " << v->stms.size() << std::endl;
    for (const auto& s : v->stms) {
        s->accept(this);
    }
}

void ExecVisitor::visit(const ast::Print* v) const {
    std::cout << "Visiting Print" << std::endl;
    std::cout << v->format;
}

void ExecVisitor::visit(const ast::CallMethod* visitable) const {
    std::cout << "Visiting CallMethod" << std::endl;
    (void)visitable;
}

void ExecVisitor::visit(const ast::Integer* visitable) const {
    std::cout << "Visiting Integer" << std::endl;
    (void)visitable;
}
void ExecVisitor::visit(const ast::Boolean* visitable) const {
    std::cout << "Visiting Boolean" << std::endl;
    (void)visitable;
}
void ExecVisitor::visit(const ast::Null* visitable) const {
    std::cout << "Visiting Null" << std::endl;
    (void)visitable;
}
void ExecVisitor::visit(const ast::Variable* visitable) const {
    std::cout << "Visiting Variable" << std::endl;
    (void)visitable;
}
void ExecVisitor::visit(const ast::AccessVariable* visitable) const {
    std::cout << "Visiting AccessVariable" << std::endl;
    (void)visitable;
}
void ExecVisitor::visit(const ast::AssignVariable* visitable) const {
    std::cout << "Visiting AssignVariable" << std::endl;
    (void)visitable;
}
void ExecVisitor::visit(const ast::Function* visitable) const {
    std::cout << "Visiting Function" << std::endl;
    (void)visitable;
}
void ExecVisitor::visit(const ast::CallFunction* visitable) const {
    std::cout << "Visiting CallFunction" << std::endl;
    (void)visitable;
}
void ExecVisitor::visit(const ast::Block* visitable) const {
    std::cout << "Visiting Block" << std::endl;
    for (const auto& s : visitable->stms) {
        s->accept(this);
    }
}
