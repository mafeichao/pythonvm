print(int)
print(str)
print(list)
print(dict)

t = type(1)
print(t)         # <class 'int'>
print(type(t))   # <class 'type'>

i = 0
while i < 5:
    t = type(t)
    print(t)     # <class 'type'>
    i = i + 1

a = int()
print(a)
b = str("hello")
print(b)
c = list()
print(c)
d = dict()
print(d)
