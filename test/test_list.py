lst = []
lst.append(1)
lst.append("hello")

# result is [1, 'hello']
lst[0] = "world"
print(lst[0])
del lst[0]
print(lst)

