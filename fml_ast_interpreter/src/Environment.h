#ifndef FML_AST_INTERPRETER_ENVIRONMENT_H
#define FML_AST_INTERPRETER_ENVIRONMENT_H

#include <map>

#include "AstNodes.h"


class Environment {
  private:
    Environment* m_parent{ nullptr };
    std::map<std::string, std::unique_ptr<ast::AST>> m_values{};
    bool m_canPassThrough{ false };
    bool m_shouldPassThrough{ false };

  public:
    Environment() = default;
    explicit Environment(Environment* parent) : m_parent(parent) {}

    void define(std::string name, std::unique_ptr<ast::AST> value, bool multPass = false);
    void set(const std::string& name, std::unique_ptr<ast::AST> value, bool multPass = false);
    [[nodiscard]] const ast::AST* get(const std::string& name, bool multPass = false) const;
    [[nodiscard]] Environment* getParent() const;
    void setParent(Environment* env);
    void setCanPassThrough(bool pass) { m_canPassThrough = pass; }
    void setShouldPassThrough(bool pass) { m_shouldPassThrough = pass; }
};


#endif //FML_AST_INTERPRETER_ENVIRONMENT_H
