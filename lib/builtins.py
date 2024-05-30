def sum(iterable, i = 0):
    temp = i
    for e in iterable:
        temp = temp + e

    return temp

class Exception(object):
    def __init__(self, *args):
        self.info = args

    def __repr__(self):
        return "Error"

class ZeroDivisionError(Exception):
    def __init__(self, *args):
        self.info = args

    def __repr__(self):
        return "divide by zero"

