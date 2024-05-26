#ifndef _UNIVERSE_HPP
#define _UNIVERSE_HPP

#include <stdio.h>

class HiInteger;
class HiObject;
class HiString;
class Klass;
class Heap;
class CodeObject;
class OopClosure;

template<typename T> class ArrayList;

class Universe {
public:
    static HiString* HiTrue;
    static HiString* HiFalse;
    static HiString* HiNone;
    static Heap*     heap;

    static CodeObject* main_code;
    static ArrayList<Klass*>* klasses;

public:
    static void genesis();
    static void destroy();
    static void oops_do(OopClosure* closure);
};

#endif

