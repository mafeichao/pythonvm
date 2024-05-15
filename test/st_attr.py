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
