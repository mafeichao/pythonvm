#include "lexer.hpp"


void test_lexer(const char* filename) {
    Lexer lexer(filename);
    Token* t = NULL;
    while ((t = lexer.next())) {
        t->print();
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: galang filename\n");
        return -1;
    }

    test_lexer(argv[1]);

    return 0;
}

