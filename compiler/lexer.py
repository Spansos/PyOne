from collections import OrderedDict
import re


class Token():
    def __init__(self, type: str, value: str):
        self.type = type
        self.value = value
    
    def __repr__(self):
        return f"<{self.type}:{repr(self.value)}>"


TOKEN_TYPES = OrderedDict()
TOKEN_TYPES['KEYWORD']      =   'fun|return|while|for|break|skip|if|else'
TOKEN_TYPES['IDENTIFIER']   =   '[a-zA-Z_][a-zA-Z0-9_]*'
TOKEN_TYPES['LITERAL']      =   '([0-9]+(\.[0-9]*)?)|\'[^\'\n]*\'?|"[^"\n]*"?|true|false'
TOKEN_TYPES['OPERATOR']     =   '(\+|-|\*|^|\/|\/\/|%|\||&|!|=|<|>)(=)?'
TOKEN_TYPES['SEPERATOR']    =   '\[|\]|\(|\)|{|}|\n|,'
TOKEN_TYPES['COMMENT']      =   '(#|\/\/).*'

for k, v in TOKEN_TYPES.items():
    TOKEN_TYPES[k] = re.compile(v)


def lex(string):
    tokens = []

    token = ''
    for char in string:
        token += char

        match = False
        for k, v in TOKEN_TYPES.items():
            if re.fullmatch(v, token):
                match = True
        
        if not match:
            for k, v in TOKEN_TYPES.items():
                if re.fullmatch(v, token[:-1]):
                    tokens.append(Token(k, token[:-1]))
                    break
            token = token[-1]

    for k, v in TOKEN_TYPES.items():
        if re.fullmatch(v, token):
            tokens.append(Token(k, token))
            break
    
    return tokens