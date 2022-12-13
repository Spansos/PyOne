import sys
import ast
ast


class Line:
    lines = []
    def __init__(self, line,  linenr, func=None):
        line = line.strip()
        self.words = line.split(' ')
        self.linenr = linenr
        if func:
            func.lines.append(self)
        else:
            Line.lines.append(self)
    
    def left(self):
        return None

    def right(self):
        return None

class Var(Line):
    types = {'str', 'int', 'float', 'list', 'tuple', 'dict', 'set', 'bool'}
    vars = {}
    def __init__(self, line, linenr):
        super().__init__(line, linenr)
        self.type = words[0]
        self.name = words[1]
        self.value = words[2]
    
    def left(self):
        return self.name
    
    def right(self):
        return f"(lambda x: lambda: x)({self.value})"

    def call(self):
        return f"{self.name}()[0]"

class Fun(Line):
    def __init__(self):
        self.lines = []

class FunCall():
    pass

# read in file
# filename = sys.argv[-1]
filename = 'compiler/test.p1'
with open(filename, 'r') as file:
    lines = file.readlines()
for i, line in enumerate(lines):
    Line(line, i)


for line in lines:
    words = line.split(' ')
    match words[0]:
        case _:
            pass

# with open('out.py', 'w') as file:
#     file.write(outstring)