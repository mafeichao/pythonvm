class Vector(object):
    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z

    def say(self):
        print(self.x)
        print(self.y)
        print(self.z)

a = Vector(1, 2, 3)
b = Vector(4, 5, 6)

a.say()
b.say()

