keys = []
values = []

class B(object):
    def __setattr__(self, k, v): 
        if k in keys:
            index = keys.index(k)
            values[index] = v 
        else:
            keys.append(k)
            values.append(v)

    def __getattr__(self, k): 
        if k in keys:
            index = keys.index(k)
            return values[index]
        else:
            return None

b = B()
b.foo = 1
b.bar = 2
print(b.foo)
print(b.bar)
b.foo = 3
print(b.foo)

class A(object):
    value = 1

a = A()
print(a.value)

#this is OK
a.field = "hello"
#  44 LOAD_CONST               2 ('hello')
#  47 LOAD_NAME                2 (a)
#  50 STORE_ATTR               4 (field)
print(a.field)
a.value = 2

b = A()
print(b.value)
A.value = 3
print(b.value)
# this is wrong, too
#print b.field
