def func(z=5):
    x = 2
    y = 5
    def say(a0=1,a1=2,a2=3,a3=4,a4=5,a5=6,a6=7,a7=8,a8=9):
        nonlocal x
        x += y
        x += z
        print(x)
    
    x += 1
    y += 1
    return say

f = func()
f()
f()

def foo():
    value = 1
    print(value)

    def bar(a = 3):
        print(value)
        inner_v = 4

        def func1():
            print(a)
            print(value)

        def func2():
            print(a)
            print(inner_v)

        return func1, func2

    return bar

f = foo()
f()
f, g = f()
f()
g()

def func(x = 5):
    def say():
        print(x)

    x = 3
    print(x)
    return say

f = func()
print("hello")
f()

def foo():
    x = 2
    def bar():
        print(x)

    x = 3

    return bar

f = foo()
f()
