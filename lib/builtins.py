def sum(iterable, i = 0):
    temp = i
    for e in iterable:
        temp = temp + e

    return temp

class Exception(object):
    def __init__(self, *args):
        self.info = args
        self.__context__ = None
        self.__traceback__ = None

    def __repr__(self):
        return "Error"

class ZeroDivisionError(Exception):
    def __init__(self, *args):
        self.info = args

    def __repr__(self):
        return "divide by zero"

class StopIteration(Exception):
    def __init__(self, *args):
        self.info = args

    def __repr__(self):
        return "stop iteration."

def range(*args):
    start = 0
    end = 0
    step = 1

    if len(args) == 1:
        end = args[0]
    elif len(args) == 2:
        start = args[0]
        end = args[1]
    elif len(args) == 3:
        start = args[0]
        end = args[1]
        step = args[2]

    if (start < end and step > 0):
        while start < end:
            yield start
            start += step
    elif (start > end and step < 0):
        while start > end:
            yield start
            start += step
    else:
        raise StopIteration

