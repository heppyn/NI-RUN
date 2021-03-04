#include "Environment.h"
#include <iostream>

void Environment::define(std::string name, std::unique_ptr<ast::AST> value, bool multPass) {
    if (m_canPassThrough && (m_shouldPassThrough || multPass)) {
        if (m_parent) {
            m_parent->define(std::move(name), std::move(value), true);
        }
        else {
            std::cerr << "Trying to pass through environment but parent is null\n";
            throw std::invalid_argument("Trying to pass through environment but parent is null");
        }
    }
    else {
        if (m_values.find(name) != m_values.end()) {
            std::cerr << "Redefining already defined value\n";
            throw std::invalid_argument("Redefining already defined value");
        }
        m_values.emplace(std::move(name), std::move(value));
    }
}

void Environment::set(const std::string& name, std::unique_ptr<ast::AST> value, bool multPass) {
    if (m_canPassThrough && (m_shouldPassThrough || multPass)) {
        if (m_parent == nullptr) {
            std::cerr << "Assigning to variable that is not defined\n";
            throw std::invalid_argument("Assigning to variable that is not defined");
        }
        else {
            m_parent->set(name, std::move(value), true);
        }
    }
    else {
        auto ptr = m_values.find(name);
        if (ptr == m_values.end()) {
            if (m_parent == nullptr) {
                std::cerr << "Assigning to variable that is not defined\n";
                throw std::invalid_argument("Assigning to variable that is not defined");
            }
            else {
                m_parent->set(name, std::move(value));
            }
        }
        else {
            ptr->second = std::move(value);
        }
    }
}

const ast::AST* Environment::get(const std::string& name, bool multPass) const {
    if (m_canPassThrough && (m_shouldPassThrough || multPass)) {
        if (m_parent == nullptr) {
            std::cerr << "Reading from not defined variable\n";
            throw std::invalid_argument("Reading from not defined variable");
        }
        else {
            return m_parent->get(name, true);
        }
    }
    else {
        auto ptr = m_values.find(name);
        if (ptr == m_values.end()) {
            if (m_parent == nullptr) {
                std::cerr << "Reading from not defined variable\n";
                throw std::invalid_argument("Reading from not defined variable");
            }
            else {
                return m_parent->get(name);
            }
        }
        else {
            return ptr->second.get();
        }
    }
}
Environment* Environment::getParent() const {
    return m_parent;
}
void Environment::setParent(Environment* env) {
    m_parent = env;
}
