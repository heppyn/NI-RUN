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

std::vector<std::unique_ptr<ast::AST>> AstParser::parseNodes(const rapidjson::Value &nodes) {
    std::vector<std::unique_ptr<ast::AST>> res{};
    assert(nodes.IsArray());

    for (auto itr = nodes.Begin(); itr != nodes.End(); ++itr) {
        res.emplace_back(parseNode(itr->GetObject()));
    }

    return res;
}

ast::AST* AstParser::parseNode(const rapidjson::Value& node) {
    assert(node.IsObject());

    if (node.HasMember("Top")) {
        auto* top = new ast::Top{};
        top->stms = parseNodes(node["Top"].GetArray());
        return top;
    }
    if (node.HasMember("CallMethod")) {
        const auto& child = node["CallMethod"].GetObject();
        auto* cm = new ast::CallMethod;
        cm->name = child["name"].GetString();
        cm->object = std::unique_ptr<ast::AST>(parseNode(child["object"].GetObject()));
        cm->arguments = parseNodes(child["arguments"].GetArray());
        return cm;
    }
    if (node.HasMember("Print")) {
        const auto& child = node["Print"].GetObject();
        auto* stm = new ast::Print;
        stm->format = child["format"].GetString();
        stm->arguments = parseNodes(child["arguments"].GetArray());
        return stm;
    }
    if (node.HasMember("Integer")) {
        auto* i = new ast::Integer{};
        i->value = node["Integer"].GetInt();
        return i;
    }

    std::cerr << "Unrecognised token" << std::endl;
    throw std::invalid_argument("Unrecognised token");
}


