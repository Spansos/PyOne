import regex

class Token():
    def __init__(self, type, value):
        self.type = type
        self.value = value
    
    def __repr__(self):
        return f"<{self.type}:{self.value}>"

TOKEN_TYPES = {
    int: '/[0-9]+/',
    float: '/[0-9]+(\.[0-9]*)/',
    
    
    
}

TOKENS = []

with open("test.p1", "r") as file:
    lines = file.read()    


