class A(object):
    def __init__(self, v):
        self.v = v

# i = 0x7FFFFFFF
i = 100000
while i > 0:
    i = i - 1
    x = A("a")

print("OK!")
