from collections import OrderedDict
import re


class Token():
    def __init__(self, type: str, value: str):
        self.type = type
        self.value = value
    
    def __repr__(self):
        return f"<{self.type}:{repr(self.value)}>"


TOKEN_TYPES = OrderedDict()

# OVERLAP
TOKEN_TYPES['KEYWORD']      =   'fun|return|while|for|break|skip|if|else'
TOKEN_TYPES['BOOL']         =   'true|false'
TOKEN_TYPES['IDENTIFIER']   =   '[a-zA-Z_][a-zA-Z0-9_]*'
# TYPES
TOKEN_TYPES['INTEGER']      =   '[0-9]+'
TOKEN_TYPES['FLOAT']        =   '([0-9]+\.[0-9]*)|\.[0-9]+'
TOKEN_TYPES['STRING']       =   '".*"|\'.*\''
# LOGICAL
TOKEN_TYPES['COMPARISON']   =   '==|!=|>=|<=|>|<'
TOKEN_TYPES['AND']          =   '&'
TOKEN_TYPES['OR']           =   '|'
TOKEN_TYPES['NOT']          =   '!'
# OPERATORS
TOKEN_TYPES['PLUS']         =   '\+'
TOKEN_TYPES['MULT']         =   '\*'
TOKEN_TYPES['POWER']        =   '\^'
TOKEN_TYPES['DIVISION']     =   '\/'
TOKEN_TYPES['INT_DIVISION'] =   '\/\/'
TOKEN_TYPES['MODULUS']      =   '%'
# DON'T KNOW
TOKEN_TYPES['ASSIGNMENT']   =   '='
TOKEN_TYPES['SEPERATOR']    =   '\[|\]|\(|\)|{|}|;|,|:'
# IRRELEVANT
TOKEN_TYPES['WHITESPACE']   =   ' +|\n+|\t+'
TOKEN_TYPES['COMMENT']      =   '(#|\/\/).*|\/\*(.|\n)*\*\/'
TOKEN_TYPES['ERROR']        =   '.|\n'

for k, v in TOKEN_TYPES.items():
    TOKEN_TYPES[k] = re.compile(v)


def lex(string):

    tokens = []
    while string:

        matches = OrderedDict()
        for k, v in TOKEN_TYPES.items():
            match = re.match(v, string)
            if match:
                matches[k] = match
        match = max(matches.items(), key=lambda item: len(item[1].group(0)))

        tokens.append(Token(match[0], match[1].group(0)))
        string = string[len(match[1].group(0)):]
    
    tokens = [token for token in tokens if token.type != 'WHITESPACE' and token.type != 'COMMENT']

    return tokens