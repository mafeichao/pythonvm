class A(object):
    def __init__(self):
        print(__class__)
        print(self.__class__)

class B(A):
    pass

print(A())