from collections import OrderedDict
import re


class Token():
    def __init__(self, type: str, value: str, program: str, start: int, end: int):
        # for actual parsing
        self.type = type
        self.value = value
        # for 'nice' errors
        self.program = program
        self.start = start
        self.end = end
    
    def __repr__(self):
        return f"<{self.type}:{repr(self.value)}@{self.start}-{self.end}>"


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
TOKEN_TYPES['FLOAT']        =   '[0-9]+\.[0-9]*|\.[0-9]+'
TOKEN_TYPES['STRING']       =   '"(?:\\\\"|[^"\n])*[^\\\\]"|\'(?:\\\\\'|[^\\\'\n])*[^\\\\]\''
TOKEN_TYPES['BOOL']         =   'true|false'
# IDENTIFIER
TOKEN_TYPES['IDENTIFIER']   =   '[a-zA-Z][a-zA-Z0-9_]*'
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
# STATEMENTS
TOKEN_TYPES['ASSIGNMENT']   =   '='
# OPERATORS
TOKEN_TYPES['PLUS']         =   '\+'
TOKEN_TYPES['MINUS']        =   '\-'
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
TOKEN_TYPES['COLON']        =   ':'
TOKEN_TYPES['COMMA']        =   ','
# IRRELEVANT
TOKEN_TYPES['WHITESPACE']   =   ' +|\n+|\t+'
TOKEN_TYPES['COMMENT']      =   '#.*|\/\*(.|\n)*\*\/'
TOKEN_TYPES['ERROR']        =   '.|\n'

for k, v in TOKEN_TYPES.items():
    TOKEN_TYPES[k] = re.compile(v)


# creates a string of tokens for a given program
def lex(program: str):

    pos = 0
    tokens = []
    while pos < len(program) and (not tokens or tokens[-1].type != 'ERROR'):

        # determine all tokens that the next bit of code might be
        matches = []
        for type, pattern in TOKEN_TYPES.items():
            match = pattern.match(program, pos)
            if not match: continue

            token = Token(
                    type    =   type,
                    value   =   match.group(),
                    program =   program,
                    start   =   match.start(),
                    end     =   match.end()-1
            )
            matches.append(token)
        
        # add the largest token (the token that has the largest capture group) to list
        # and set pos to the end of that token
        token = max(matches, key=lambda item: item.end)
        tokens.append(token)
        pos = token.end + 1
    
    # remove tokens that will be ignored by the parser
    tokens = [token for token in tokens if token.type != 'WHITESPACE' and token.type != 'COMMENT']

    return tokens