import lexer

def parseExpression(tokens):
    exprTokens = []
    while (token := tokens.pop()).type != 'SEMICOLON' and token.type != 'ENDOFFILE':
        exprTokens.append(token)
    
    print(exprTokens)
    return tokens


def parseStart(tokens):
    if not tokens:
        return tokens
    tokens = parseExpression(tokens)
    tokens = parseStart(tokens)

with open('test.p1') as file:
    string = file.read()

tokens = lexer.lex(string)
parseStart(tokens)