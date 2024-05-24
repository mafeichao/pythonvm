def foo():
    print("hello")
	
foo()

def call_ex(a, b, c, d):
    return a + b + c + d

args = [3, 4]
a = [1, ]
b = (2, )
print(call_ex(*a, *b, *args))

class A(object):
    def __init__(self, x):
        self.x = x

    def __call__(self):
        return self.x


a = A(1)
print(a())
