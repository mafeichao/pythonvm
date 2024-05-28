class Vector(object):
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __add__(self, v):
        return Vector(self.x + v.x, self.y + v.y)

    def __len__(self):
        return self.x * self.x + self.y * self.y

    def __str__(self):
        return "hello"

    def __repr__(self):
        return "world"

v = Vector(3, 4)
print(v)
print(len(v))
print(len("hello"))

