print(int)
print(str)
print(list)
print(dict)

t = type(1)
print(t)         # <type int>
print(type(t))   # <type type>

i = 0
while i < 5:
    t = type(t)
    print(t)     # <type type>
    i = i + 1

