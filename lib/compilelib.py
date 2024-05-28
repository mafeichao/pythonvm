import py_compile as c
import sys

def compilelib(d):
    c.compile(d + "/builtins.py", "builtins.pyc")
    c.compile(d + "/math.py", "math.pyc")

if __name__ == "__main__":
    compilelib(sys.argv[1])

