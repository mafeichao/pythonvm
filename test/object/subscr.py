class A(object):
    def __init__(self, *args):
        self.attrs = dict()
        n = len(args)
        i = 0
        while (i < n):
            a = args[i]
            i += 1
            b = args[i]
            i += 1
            self.attrs[a] = b
            
    def __getitem__(self, key):
        if key in self.attrs:
            return self.attrs[key]
        else:
            return "Error"

    def __setitem__(self, key, value):
        self.attrs[key] = value

    def __delitem__(self, key):
        del self.attrs[key]

    def __str__(self):
        return "object of A"

a = A("hello", "hi", "how are you", "fine")
print(a)
print(a["hello"])
print(a["how are you"])
a["one"] = 1
print(a["one"])
del a["one"]
print(a["one"])
print(a.attrs)
