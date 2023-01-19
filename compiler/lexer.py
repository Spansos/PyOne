from collections import OrderedDict
import re


class Token():
    def __init__(self, type: str, value: str):
        self.type = type
        self.value = value
    
    def __repr__(self):
        return f"<{self.type}:{repr(self.value)}>"


TOKEN_TYPES = OrderedDict()

# KEYWORDS
TOKEN_TYPES['FUN']          =   'fun'
TOKEN_TYPES['RETURN']       =   'return'
TOKEN_TYPES['WHILE']        =   'while'
TOKEN_TYPES['FOR']          =   'for'
TOKEN_TYPES['BREAK']        =   'break'
TOKEN_TYPES['SKIP']         =   'skip'
TOKEN_TYPES['IF']           =   'if'
TOKEN_TYPES['ELSE']         =   'else'
# TYPES
TOKEN_TYPES['INTEGER']      =   '[0-9]+'
TOKEN_TYPES['FLOAT']        =   '([0-9]+\.[0-9]*)|\.[0-9]+'
TOKEN_TYPES['STRING']       =   '".*"|\'.*\''
TOKEN_TYPES['BOOL']         =   'true|false'
# IDENTIFIER
TOKEN_TYPES['IDENTIFIER']   =   '[a-zA-Z_][a-zA-Z0-9_]*'
# LOGICAL
TOKEN_TYPES['EQUALS']       =   '=='
TOKEN_TYPES['NOT_EQUALS']   =   '!='
TOKEN_TYPES['GREATER_EQUAL']=   '>='
TOKEN_TYPES['LESSER_EQUAL'] =   '<='
TOKEN_TYPES['GREATER']      =   '>'
TOKEN_TYPES['LESSER']       =   '<'
TOKEN_TYPES['AND']          =   '&'
TOKEN_TYPES['OR']           =   '\|'
TOKEN_TYPES['NOT']          =   '!'
# STATEMENTs
TOKEN_TYPES['ASSIGNMENT']   =   '='
# OPERATORS
TOKEN_TYPES['PLUS']         =   '\+'
TOKEN_TYPES['MULT']         =   '\*'
TOKEN_TYPES['POWER']        =   '\^'
TOKEN_TYPES['DIVISION']     =   '\/'
TOKEN_TYPES['INT_DIVISION'] =   '\/\/'
TOKEN_TYPES['MODULUS']      =   '%'
# SEPERATORS
TOKEN_TYPES['SQUARE_OPEN']  =   '\['
TOKEN_TYPES['SQUARE_CLOSE'] =   ']'
TOKEN_TYPES['ROUND_OPEN']   =   '\('
TOKEN_TYPES['ROUND_CLOSE']  =   '\)'
TOKEN_TYPES['CURLY_OPEN']   =   '{'
TOKEN_TYPES['CURLY_CLOSE']  =   '}'
TOKEN_TYPES['SEMICOLON']    =   ';'
TOKEN_TYPES['COMMA']        =   ','
TOKEN_TYPES['ENDOFFILE']    =   '\Z'
# IRRELEVANT
TOKEN_TYPES['WHITESPACE']   =   ' +|\n+|\t+'
TOKEN_TYPES['COMMENT']      =   '(#|\/\/).*|\/\*(.|\n)*\*\/'
TOKEN_TYPES['ERROR']        =   '.|\n'

for k, v in TOKEN_TYPES.items():
    TOKEN_TYPES[k] = re.compile(v)


def lex(string):
    
    tokens = []
    while (not tokens) or tokens[-1].type != 'ENDOFFILE':

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