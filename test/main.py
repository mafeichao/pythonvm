import py_compile as c
import sys
import os

def compile_dir(d):
    for name in os.listdir(d):
        o = name
        name = d + "/" + name
        if (os.path.isdir(name)):
            compile_dir(name)
        elif os.path.isfile(name):
            if name[-3:] != ".py":
                continue
            print("compiling " + name)
            c.compile(name, o + "c")

if __name__ == "__main__":
    compile_dir(sys.argv[1])

