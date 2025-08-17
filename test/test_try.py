i = 0
while i < 10:
    i += 1
    try:
        if (i == 5):
            break
        print(i)

    finally:
        print("hello")
        print("world")

try:
    1/0
finally:
    print("finally")
    
try:
    1/0
except Exception as e:
    print(e)
finally:
    print("finally")

def foo():
    try:
        return "haha"
    finally:
        print("haha world")

print(foo())

for i in range(5):
    try:
        if (i < 3):
            continue
        print(i)
    finally:
        print("to be continued")
