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
