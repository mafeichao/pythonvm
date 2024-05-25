#ifndef _UNIVERSE_HPP
#define _UNIVERSE_HPP

#include <stdio.h>

class HiInteger;
class HiObject;
class HiString;
class Heap;

class Universe {
public:
    static HiString* HiTrue;
    static HiString* HiFalse;
    static HiString* HiNone;
    static Heap*     heap;

public:
    static void genesis();
    static void destroy();
};

#endif

