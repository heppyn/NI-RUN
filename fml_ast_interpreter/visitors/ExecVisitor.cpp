#include "ExecVisitor.h"
#include <iostream>
#include <memory>

std::unique_ptr<ast::AST> ExecVisitor::visit(const ast::Top* v) const {
    std::cout << "Visiting Top" << std::endl;
    std::cout << "Size of stms: " << v->stms.size() << std::endl;
    for (const auto& s : v->stms) {
        s->accept(this);
    }

    return nullptr;
}

std::unique_ptr<ast::AST> ExecVisitor::visit(const ast::Print* v) const {
    std::cout << "Visiting Print" << std::endl;
    size_t args = 0;
    for (size_t i = 0; i < v->format.size(); i++) {
        if (v->format[i] == '~') {
            std::cout << evaluate(v->arguments[args].get())->toString();
            args++;
            continue;
        }
        if (v->format[i] == '\\') {
            if (i + 2 < v->format.size() && v->format[i + 1] == '\\') {
                if (v->format[i + 2] == '~') {
                    std::cout << "~";
                }
                else if (v->format[i + 2] == 'n') {
                    std::cout << "\n";
                }
                else if (v->format[i + 2] == 't') {
                    std::cout << "\t";
                }
                else if (i + 3 < v->format.size() && v->format[i + 2] == '\\' && v->format[i + 3] == '\\') {
                    std::cout << "\\";
                    i += 1;
                }
                else {
                    std::cerr << "Incorrect escape char: '"
                              << v->format[i + 2] << "' in " << v->format << '\n';
                    throw std::invalid_argument("Incorrect escape char");
                }
                i += 2;
            }
            else {
                std::cerr << "Incorrect format string: '" << v->format << "'\n";
                throw std::invalid_argument("Incorrect format string");
            }
            continue;
        }
        std::cout << v->format[i];
    }
    return std::unique_ptr<ast::Null>();
}

std::unique_ptr<ast::AST> ExecVisitor::visit(const ast::CallMethod* visitable) const {
    std::cout << "Visiting CallMethod" << std::endl;
    (void)visitable;
    return nullptr;
}

std::unique_ptr<ast::AST> ExecVisitor::visit(const ast::Integer* visitable) const {
    return std::make_unique<ast::Integer>(visitable->value);
}
std::unique_ptr<ast::AST> ExecVisitor::visit(const ast::Boolean* visitable) const {
    return std::make_unique<ast::Boolean>(visitable->value);
}
std::unique_ptr<ast::AST> ExecVisitor::visit(const ast::Null*) const {
    return std::make_unique<ast::Null>();
}
std::unique_ptr<ast::AST> ExecVisitor::visit(const ast::Variable* visitable) const {
    std::cout << "Visiting Variable" << std::endl;
    (void)visitable;
    return nullptr;
}
std::unique_ptr<ast::AST> ExecVisitor::visit(const ast::AccessVariable* visitable) const {
    std::cout << "Visiting AccessVariable" << std::endl;
    (void)visitable;
    return nullptr;
}
std::unique_ptr<ast::AST> ExecVisitor::visit(const ast::AssignVariable* visitable) const {
    std::cout << "Visiting AssignVariable" << std::endl;
    (void)visitable;
    return nullptr;
}
std::unique_ptr<ast::AST> ExecVisitor::visit(const ast::Function* visitable) const {
    std::cout << "Visiting Function" << std::endl;
    (void)visitable;
    return nullptr;
}
std::unique_ptr<ast::AST> ExecVisitor::visit(const ast::CallFunction* visitable) const {
    std::cout << "Visiting CallFunction" << std::endl;
    (void)visitable;
    return nullptr;
}
std::unique_ptr<ast::AST> ExecVisitor::visit(const ast::Block* visitable) const {
    std::cout << "Visiting Block" << std::endl;
    (void)visitable;
    for (const auto& s : visitable->stms) {
        s->accept(this);
    }

    return nullptr;
}
std::unique_ptr<ast::AST> ExecVisitor::evaluate(const ast::AST* stm) const {
    return stm->accept(this);
}
