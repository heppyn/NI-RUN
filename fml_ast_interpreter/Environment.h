#ifndef FML_AST_INTERPRETER_ENVIRONMENT_H
#define FML_AST_INTERPRETER_ENVIRONMENT_H

#include <map>

#include "AstNodes.h"


class Environment {
  private:
    Environment* m_parent{ nullptr };
    std::map<std::string, std::unique_ptr<ast::AST>> m_values{};

  public:
    Environment() = default;
    explicit Environment(Environment* parent) : m_parent(parent) {}

    void define(std::string name, std::unique_ptr<ast::AST> value);
    void set(const std::string& name, std::unique_ptr<ast::AST> value);
    [[nodiscard]] const ast::AST* get(const std::string& name) const;
};


#endif //FML_AST_INTERPRETER_ENVIRONMENT_H
