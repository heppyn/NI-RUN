#ifndef FML_AST_INTERPRETER_ASTPARSER_H
#define FML_AST_INTERPRETER_ASTPARSER_H

#include <string_view>
#include <memory>

#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

#include "AstNodes.h"

class AstParser {
public:
    static ast::Top parse(std::string_view path);

private:
    static rapidjson::Document loadJson(std::string_view path);
    static ast::AST* parseNode(const rapidjson::Value& node);
    static std::vector<std::unique_ptr<ast::AST>> parseNodes(const rapidjson::Value& nodes);
};


#endif //FML_AST_INTERPRETER_ASTPARSER_H
