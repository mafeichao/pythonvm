#include "util/bufferedInputStream.hpp"
#include "code/binaryFileParser.hpp"
#include "runtime/universe.hpp"
#include "runtime/interpreter.hpp"

int main(int argc, char** argv) {
    if (argc <= 1) {
        printf("vm need a parameter : filename\n");
        return 0;
    }

    Universe::genesis();

    BufferedInputStream stream(argv[1]);
    BinaryFileParser parser(&stream);
    Universe::main_code = parser.parse();

    Interpreter::get_instance()->add_search_path(argv[1]);
    Interpreter::get_instance()->run(Universe::main_code);

    Universe::destroy();

    return 0;
}

