def foo():
    try:
        print("in try")
        return 3
    except Exception as e:
        print(e)
    finally:
        print("in finally")

def bar():
    try:
        return 3
    finally:
        i = 0
        while i < 10:
            print(i)
            if (i == 5):
                break
            i += 1

print(foo())
print(bar())

