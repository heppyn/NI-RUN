#include "ExecVisitor.h"
#include "scope_guard.hpp"
#include <iostream>
#include <memory>

std::unique_ptr<ast::AST> ExecVisitor::visit(const ast::Top* v) {
    //    std::cout << "Visiting Top" << std::endl;
    //    std::cout << "Size of stms: " << v->stms.size() << std::endl;
    size_t i = 0;
    for (; i < v->stms.size() - 1; i++) {
        evaluate(v->stms[i].get());
    }
    auto res = evaluate(v->stms[i].get());

    return res;
}

std::unique_ptr<ast::AST> ExecVisitor::visit(const ast::Print* v) {
    //    std::cout << "Visiting Print. Format: '"<< v->format << "'\n";
    // count expected args
    unsigned int expArgs = 0;
    for (const auto c : v->format) {
        if (c == '~')
            expArgs++;
    }
    if (expArgs != v->arguments.size()) {
        std::cerr << "Provided wrong number of arguments to print. "
                  << "Provided: " << v->arguments.size() << " expected: " << expArgs << '\n';
        throw std::invalid_argument("Provided wrong number of arguments to print");
    }
    // Evaluate args first
    std::vector<std::unique_ptr<ast::AST>> eval;
    eval.reserve(expArgs);
    for (const auto& argument : v->arguments) {
        eval.emplace_back(evaluate(argument.get()));
    }

    size_t args = 0;
    for (size_t i = 0; i < v->format.size(); i++) {
        if (v->format[i] == '~') {
            std::cout << eval[args]->toString();
            args++;
            continue;
        }
        if (v->format[i] == '\\') {
            if (i + 1 < v->format.size()) {
                if (v->format[i + 1] == '~') {
                    std::cout << "~";
                }
                else if (v->format[i + 1] == 'n') {
                    std::cout << "\n";
                }
                else if (v->format[i + 1] == '"') {
                    std::cout << "\"";
                }
                else if (v->format[i + 1] == 'r') {
                    std::cout << "\r";
                }
                else if (v->format[i + 1] == 't') {
                    std::cout << "\t";
                }
                else if (v->format[i + 1] == '\\') {
                    std::cout << "\\";
                }
                else {
                    std::cerr << "Incorrect escape char: '"
                              << v->format[i + 1] << "' in " << v->format << '\n';
                    throw std::invalid_argument("Incorrect escape char");
                }
                i += 1;
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

std::unique_ptr<ast::AST> ExecVisitor::visit(const ast::CallMethod* visitable) {
    //    std::cout << "Visiting CallMethod" << std::endl;
    (void)visitable;
    return nullptr;
}

std::unique_ptr<ast::AST> ExecVisitor::visit(const ast::Integer* visitable) {
    return std::make_unique<ast::Integer>(visitable->value);
}
std::unique_ptr<ast::AST> ExecVisitor::visit(const ast::Boolean* visitable) {
    return std::make_unique<ast::Boolean>(visitable->value);
}
std::unique_ptr<ast::AST> ExecVisitor::visit(const ast::Null*) {
    return std::make_unique<ast::Null>();
}
std::unique_ptr<ast::AST> ExecVisitor::visit(const ast::Variable* visitable) {
    //    std::cout << "Visiting Variable" << std::endl;
    auto val = evaluate(visitable->value.get());
    m_env->define(visitable->name, val->clone());
    return val;
}
std::unique_ptr<ast::AST> ExecVisitor::visit(const ast::AccessVariable* visitable) {
    //    std::cout << "Visiting AccessVariable" << std::endl;
    // Should be basically clone
    return evaluate(m_env->get(visitable->name));
}
std::unique_ptr<ast::AST> ExecVisitor::visit(const ast::AssignVariable* visitable) {
    //    std::cout << "Visiting AssignVariable" << std::endl;
    auto val = evaluate(visitable->value.get());
    m_env->set(visitable->name, val->clone());
    return val;
}
std::unique_ptr<ast::AST> ExecVisitor::visit(const ast::Function* visitable) {
    //    std::cout << "Visiting Function" << std::endl;
    m_env->define(visitable->name, visitable->clone());
    return std::make_unique<ast::Null>();
}
std::unique_ptr<ast::AST> ExecVisitor::visit(const ast::CallFunction* visitable) {
    //    std::cout << "Visiting CallFunction" << std::endl;
    auto* func = dynamic_cast<const ast::Function*>(m_env->get(visitable->name));
    if (func->parameters.size() != visitable->arguments.size()) {
        std::cerr << "Provided wrong number of arguments. Provided: " << visitable->arguments.size()
                  << " Expected: " << func->parameters.size() << '\n';
        throw std::invalid_argument("Provided wrong number of arguments");
    }
    // if scope is possible to pass through, it should pass through it
    m_env->setShouldPassThrough(true);
    beginScope();
    // any function call in this scope should ignore this environment
    m_env->setCanPassThrough(true);
    // function call done, scope should not be passed thru ane more
    // order of guard definitions is important
    auto passThruGuard = sg::make_scope_guard([this]() { this->m_env->setShouldPassThrough(false); });
    auto scopeGuard = sg::make_scope_guard([this]() { this->endScope(); }); // endScope()

    for (size_t i = 0; i < func->parameters.size(); i++) {
        // define parameters as new vars
        m_env->define(func->parameters[i], evaluate(visitable->arguments[i].get()));
    }

    // execute function body
    auto res = evaluate(func->body.get());

    return res;
}
std::unique_ptr<ast::AST> ExecVisitor::visit(const ast::Block* visitable) {
    //    std::cout << "Visiting Block" << std::endl;
    beginScope();
    // any function call in this scope should ignore this environment
    m_env->setCanPassThrough(true);
    auto guard = sg::make_scope_guard([this]() { this->endScope(); }); // endScope()

    std::unique_ptr<ast::AST> res = std::make_unique<ast::Null>();
    size_t i = 0;
    for (; i < visitable->stms.size(); i++) {
        res = evaluate(visitable->stms[i].get());
    }

    return res;
}
std::unique_ptr<ast::AST> ExecVisitor::visit(const ast::Loop* visitable) {
    std::unique_ptr<ast::AST> res = std::make_unique<ast::Null>();
    while (evaluate(visitable->condition.get())->isTruthy()) {
        res = evaluate(visitable->body.get());
    }
    return res;
}
std::unique_ptr<ast::AST> ExecVisitor::visit(const ast::Conditional* visitable) {
    if (evaluate(visitable->condition.get())->isTruthy()) {
        return evaluate(visitable->consequent.get());
    }
    return evaluate(visitable->alternative.get());
}
std::unique_ptr<ast::AST> ExecVisitor::evaluate(const ast::AST* stm) {
    return stm->accept(this);
}
void ExecVisitor::beginScope() {
    m_env->setParent(m_env_prev);
    m_env_prev = m_env;
    m_env = new Environment(m_env_prev);
}
void ExecVisitor::endScope() {
    delete m_env;
    m_env = m_env_prev;
    m_env_prev = m_env->getParent();
}
