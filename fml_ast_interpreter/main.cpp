#include "AstParser.h"
#include "visitors/ExecVisitor.h"
#include <iostream>

int main(int argc, char** argv) {
    std::string_view filePath = "../parsed.json";
    if (argc >= 2) {
        if (strcmp(argv[1], "--help") == 0) {
            std::cout << "Run with no parameters. File '../parsed.json' will be executed.\n"
                      << "Execute file by executable -r <file path>\n";
            return 0;
        }
        if (strcmp(argv[1], "-r") == 0) {
            if (argc == 2) {
                std::cout << "Specify file to be executed.\n";
                return 1;
            }
            filePath = argv[2];
        }
    }

    ast::Top top = AstParser::parse(filePath);

    ExecVisitor v;
    top.accept(&v);

    return 0;
}
