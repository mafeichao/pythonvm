class A(object):
    def __init__(self, v):
        self.v = v

# i = 0x7FFFFFFF
#i = 10000
#while i > 0:
#    i = i - 1
#    x = A("a")

d = {3 : 4}

i = 0
a = 0
while i < 100000:
    a += 1
    i += 1

print(d)
