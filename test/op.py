class Vector(object):
    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z

    def __repr__(self):
        return "Vector(%d, %d, %d)" % (self.x, self.y, self.z)

    def __add__(self, v):
        return Vector(self.x + v.x, self.y + v.y, self.z + v.z)

v1 = Vector(1, 2, 3)
v2 = Vector(3, 4, 5)
v3 = v1 + v2
print(v3.x)
print(v3.y)
print(v3.z)
