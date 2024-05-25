#include "util/bufferedInputStream.hpp"
#include "code/binaryFileParser.hpp"
#include "runtime/universe.hpp"
#include "runtime/interpreter.hpp"
#include "runtime/stringTable.hpp"

int main(int argc, char** argv) {
    if (argc <= 1) {
        printf("vm need a parameter : filename\n");
        return 0;
    }

    Universe::genesis();

    BufferedInputStream stream(argv[1]);
    BinaryFileParser parser(&stream);
    CodeObject* main_code = parser.parse();

    Interpreter::get_instance()->run(main_code);

    StringTable::destroy();
    Interpreter::destroy();
    Universe::destroy();

    return 0;
}

