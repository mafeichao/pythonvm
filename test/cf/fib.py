a = 1 
b = 0 
i = 0 

while i < 10: 
    print(a) 
    t = a 
    a = a + b 
    b = t 

    i = i + 1 

def fib(n):
    i = 0
    a = 1
    b = 0
    while i < n:
        yield a
        t = a
        a += b
        b = t
        i += 1

for i in fib(10):
    print(i)

