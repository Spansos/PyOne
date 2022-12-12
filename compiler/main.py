import sys

class Line:
    lines = []
    def __init__(self, line,  linenr):
        line = line.strip()
        self.words = line.split(' ')
        self.linenr = linenr
        Line.lines.append(self)
    
    def left(self):
        return None

    def right(self):
        return None

class Var(Line):
    types = 
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

class Fun:
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