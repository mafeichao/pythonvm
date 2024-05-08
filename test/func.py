def foo(a, b, c):
    return a + b / c

print(foo(1, c = 2, b = 6))

def sum(a, b, *args):
    t = a + b + a
    for i in args:
        t += i
    return t

print(sum(1, 2, 3, 4))

def calc(a, b, *args, **kwargs):
    coeff = kwargs.get("coeff")

    if coeff is None:
        return 0

    t = a + b
    for i in args:
        t += i

    return coeff * t

print(calc(1, 2, 3, 4, coeff = 2))
print(calc(b = 1, a = 2, c = 3, coeff = 2))
