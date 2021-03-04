#include "AstParser.h"

#include <fstream>
#include <iostream>

ast::Top AstParser::parse(std::string_view path) {
    auto document = loadJson(path);
    assert(document.HasMember("Top"));

    ast::Top top;
    top.stms = parseNodes(document["Top"].GetArray());

    return top;
}

rapidjson::Document AstParser::loadJson(std::string_view path) {
    std::ifstream stream(path.begin());
    rapidjson::IStreamWrapper isw(stream);

    rapidjson::Document document;
    if (document.ParseStream(isw).HasParseError()) {
        std::cerr << "Parsing error" << std::endl;
        throw std::invalid_argument("Invalid JSON file");
    }
    return document;
}

std::vector<std::unique_ptr<ast::AST>> AstParser::parseNodes(const rapidjson::Value& nodes) {
    std::vector<std::unique_ptr<ast::AST>> res{};
    assert(nodes.IsArray());

    for (auto itr = nodes.Begin(); itr != nodes.End(); ++itr) {
        if (itr->IsObject()) {
            res.emplace_back(parseNode(itr->GetObject()));
        }
        else {
            assert(itr->IsString());
            //            std::cout << "Non object type: " << itr->GetString() << '\n';
            res.emplace_back(parseNode(itr->GetString()));
        }
    }

    return res;
}

ast::AST* AstParser::parseNode(const rapidjson::Value& node) {
    assert(node.IsObject());

    if (node.HasMember("CallMethod")) {
        const auto& child = node["CallMethod"].GetObject();
        auto* stm = new ast::CallMethod;
        stm->name = child["name"].GetString();
        stm->object = std::unique_ptr<ast::AST>(parseNode(child, "object"));
        stm->arguments = parseNodes(child["arguments"].GetArray());
        return stm;
    }
    if (node.HasMember("Print")) {
        const auto& child = node["Print"].GetObject();
        auto* stm = new ast::Print;
        stm->format = child["format"].GetString();
        stm->arguments = parseNodes(child["arguments"].GetArray());
        return stm;
    }
    if (node.HasMember("Integer")) {
        auto* stm = new ast::Integer;
        stm->value = node["Integer"].GetInt();
        return stm;
    }
    if (node.HasMember("Boolean")) {
        auto* stm = new ast::Boolean;
        stm->value = node["Boolean"].GetBool();
        return stm;
    }
    if (node.HasMember("Variable")) {
        const auto& child = node["Variable"].GetObject();
        auto* stm = new ast::Variable;
        stm->name = child["name"].GetString();
        stm->value = std::unique_ptr<ast::AST>(parseNode(child, "value"));
        return stm;
    }
    if (node.HasMember("AccessVariable")) {
        const auto& child = node["AccessVariable"].GetObject();
        auto* stm = new ast::AccessVariable;
        stm->name = child["name"].GetString();
        return stm;
    }
    if (node.HasMember("AssignVariable")) {
        const auto& child = node["AssignVariable"].GetObject();
        auto* stm = new ast::AssignVariable;
        stm->name = child["name"].GetString();
        stm->value = std::unique_ptr<ast::AST>(parseNode(child, "value"));
        return stm;
    }
    if (node.HasMember("Function")) {
        const auto& child = node["Function"].GetObject();
        auto* stm = new ast::Function;
        stm->name = child["name"].GetString();
        stm->parameters = details::getObjects<std::string>(child["parameters"].GetArray());
        stm->body = std::unique_ptr<ast::AST>(parseNode(child, "body"));
        return stm;
    }
    if (node.HasMember("CallFunction")) {
        const auto& child = node["CallFunction"].GetObject();
        auto* stm = new ast::CallFunction;
        stm->name = child["name"].GetString();
        stm->arguments = parseNodes(child["arguments"].GetArray());
        return stm;
    }
    if (node.HasMember("Block")) {
        auto* stm = new ast::Block;
        stm->stms = parseNodes(node["Block"].GetArray());
        return stm;
    }
    if (node.HasMember("Loop")) {
        const auto& child = node["Loop"].GetObject();
        auto* stm = new ast::Loop;
        stm->condition = std::unique_ptr<ast::AST>(parseNode(child, "condition"));
        stm->body = std::unique_ptr<ast::AST>(parseNode(child, "body"));
        return stm;
    }
    if (node.HasMember("Conditional")) {
        const auto& child = node["Conditional"].GetObject();
        auto* stm = new ast::Conditional;
        stm->condition = std::unique_ptr<ast::AST>(parseNode(child, "condition"));
        stm->consequent = std::unique_ptr<ast::AST>(parseNode(child, "consequent"));
        stm->alternative = std::unique_ptr<ast::AST>(parseNode(child, "alternative"));
        return stm;
    }


    // should not be found anywhere - check at last
    if (node.HasMember("Top")) {
        std::cout << "Found Top token\n";
        auto* stm = new ast::Top;
        stm->stms = parseNodes(node["Top"].GetArray());
        return stm;
    }

    std::cerr << "Unrecognised token: '" << node.MemberBegin()->name.GetString() << "'\n";
    throw std::invalid_argument("Unrecognised token");
}
ast::AST* AstParser::parseNode(std::string_view node) {
    if (node.compare("Null") == 0) {
        return new ast::Null;
    }

    std::cerr << "Unrecognised string: '" << node << "'\n";
    throw std::invalid_argument("Unrecognised string");
}
ast::AST* AstParser::parseNode(const rapidjson::Value& node, const char* name) {
    if (node[name].IsObject()) {
        return parseNode(node[name]);
    }
    if (node[name].IsString()) {
        return parseNode(node[name].GetString());
    }

    std::cerr << "Unrecognised token - not a string nor object" << '\n';
    throw std::invalid_argument("Unrecognised token");
}

template<>
std::vector<std::string> details::getObjects(const rapidjson::Value& array) {
    assert(array.IsArray());
    std::vector<std::string> res;

    for (rapidjson::SizeType i = 0; i < array.Size(); i++) {
        res.emplace_back(array[i].GetString());
    }

    return res;
}
