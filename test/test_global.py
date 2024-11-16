global x
x = 0

def func():
    global x
    x = 1

func()
print(x)

y = 2
def foo():
    z = 0
    print(y)

foo()