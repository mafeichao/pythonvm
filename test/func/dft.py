# test_default.py
def foo(a, b = 1, c = 2):
    return a + b + c
	
print(foo(10))
print(foo(100, 20, 30))

