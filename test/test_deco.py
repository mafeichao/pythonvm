print(3)

def call_cnt(fn):
    count = 0
    cnt = [0, ]
    def inner_func(*args):
        cnt[0] += 1
        nonlocal count
        count += 1
        print("count:" + str(cnt[0]) + "," + str(count))
        return fn(*args)

    return inner_func

@call_cnt
def add(a, b = 2):
    return a + b

print(add(1, 2))
print(add(2, 3))

@call_cnt
def call_ex(a, b, c, d):
    return a + b + c + d

args = [3, 4]
a = [1, ]
b = (2, )
print(call_ex(*a, *b, *args))
