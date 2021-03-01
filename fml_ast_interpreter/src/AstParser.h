#ifndef FML_AST_INTERPRETER_ASTPARSER_H
#define FML_AST_INTERPRETER_ASTPARSER_H

#include <memory>
#include <string_view>

#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

#include "AstNodes.h"

class AstParser {
  public:
    static ast::Top parse(std::string_view path);

  private:
    static rapidjson::Document loadJson(std::string_view path);
    static ast::AST* parseNode(const rapidjson::Value& node);
    static ast::AST* parseNode(const rapidjson::Value& node, const char* name);
    static ast::AST* parseNode(std::string_view node);
    static std::vector<std::unique_ptr<ast::AST>> parseNodes(const rapidjson::Value& nodes);
};

namespace details {
template<typename T>
std::vector<T> getObjects(const rapidjson::Value&) = delete;
template<>
std::vector<std::string> getObjects(const rapidjson::Value&);
} // namespace details


#endif //FML_AST_INTERPRETER_ASTPARSER_H
