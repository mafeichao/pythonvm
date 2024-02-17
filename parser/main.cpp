#include "lexer.hpp"
#include "parser.hpp"
#include "codegen.hpp"
#include "codeobj.hpp"

void test_lexer(const char* filename) {
    Lexer lexer(filename);
    Token* t = NULL;
    while ((t = lexer.next())) {
        t->print();
    }
}

void test_parser(const char* filename) {
    Lexer lexer(filename);
    Parser parser(&lexer);

    CodeGen gen;
    gen.visit(parser.parse());
    CodeObject* co = gen.make_code_object();

    CodeObject::write_to_file(string(filename).append("c"), *co);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: galang filename\n");
        return -1;
    }

    test_lexer(argv[1]);
    test_parser(argv[1]);

    return 0;
}

