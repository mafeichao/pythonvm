class A(object):
    def say(self):
        print("I am A")

class B(A):
    def say(self):
        print("I am B")

class C(A):
    pass

b = B()
c = C()

b.say()    # "I am B"
c.say()    # "I am A"

class A(object):pass
class B(object):pass
class C(object):pass
class E(A,B):pass
class F(B,C):pass
class G(E,F):pass
print(G.__mro__)

A = type('A', (object,), {})
B = type('B', (object,), {})
C = type('C', (object,), {})
D = type('D', (object,), {})
E = type('E', (object,), {})
K1 = type('K1', (A, B, C), {})
K2 = type('K2', (D, B, E), {})
K3 = type('K3', (D, A), {})
Z = type('Z', (K1, K2,K3), {})
print(Z.__mro__)
