a = {1 : 100, 2 : 200, 4 : 400}
b = {3 : 100, 4 : 400}

for k in a:
    print(k)

for k in a.keys():
    print(k)
    print(a[k])

print(3 in b.keys())
