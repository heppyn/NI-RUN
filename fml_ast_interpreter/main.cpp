#include "AstParser.h"
#include "visitors/ExecVisitor.h"

int main(int, char*[]) {
    ast::Top top = AstParser::parse("../../parsed.json");
    (void)top;

    ExecVisitor v;
    top.accept(&v);

    return 0;
}
