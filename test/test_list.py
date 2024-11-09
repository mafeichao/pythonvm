la = [1, "hello"]
la = ["hello", "world"]

if "hello" in la:
    print("exist")
else:
    print("none")

print(la[1])

l = [4, 1, 2, 3]
l.append(10)
l[0] = 5
del l[3]
l.pop()
l.remove(2)
l.reverse()
l.sort()
print(l)
print(l < la)

for x in l:
    print(x)
