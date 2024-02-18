#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

class HiString;

class Interpreter {
private:
    int *_stack;

public:
    Interpreter();

    void run(HiString* codes);
};

#endif

