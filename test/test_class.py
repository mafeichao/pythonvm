print(list)                  # <type 'list'>

lst = list()
print(lst)                   # []
lst.append(1)
print(lst)                   # [1]
print(isinstance(lst, list)) # True

t = type(1)
print(t)         # <class 'int'>
print(type(t))   # <class 'type'>

i = 0
while i < 5:
    t = type(t)
    print(t)     # <class 'type'>
    i = i + 1

a = int()
#  0 LOAD_NAME                0 (int)
#  3 CALL_FUNCTION            0
#  6 STORE_NAME               1 (a)

print(a)
b = str("hello")
print(b)
c = list()
print(c)
d = dict()
print(d)
 
class A(object):
    def __init__(self):
        print("In A")

    def __getattr__(self):
        return 0

    def __add__(self, a):
        return 100

class B(A):
    pass

a = A()
print (a + a)

def foo(self, rhs):
    return 200

b = B()
b.__add__ = foo
print (b + b)
