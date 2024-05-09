def func():
    x = 2
    
    def say():
        nonlocal x
        x += 1
        print(x)

    def bar():
        nonlocal x
        x += 1
        print(x)

    return say, bar

f,g = func()
f()
g()

