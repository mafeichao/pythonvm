zero = lambda f : lambda x : x
one = lambda f : lambda x : f(x)
inc = lambda x : x + 1
add = lambda m : lambda n : lambda f : lambda x : m(f)(n(f)(x))
mul = lambda m : lambda n : lambda f : lambda x : m(n(f))(x)


two = add(one)(one)
four = add(two)(two)

eight = mul(two)(four)

print(eight(inc)(0))
