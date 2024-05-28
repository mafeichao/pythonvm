d = {1 : "hello", "world" : 2}
print(d)
print(d[1])
print(d["world"])

a = 1
b = 2
d = {a : "hello", b : "world"}
print(d)

d = {1 : "hello"}
d.setdefault(1, 2)
d.setdefault(2, 3)

print(d[1])   # "hello"
print(d[2])   # 3

print(d.pop(1))
del d[2]
print(d)

print(d.pop(1, 100))
