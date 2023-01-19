import lexer

def parseExpression(tokens):
    n_tokens, node = tokens.copy()[1:], []

    cur_bal = 0
    rnd_bal = 0
    sqr_bal = 0

    return n_tokens, node, False


def parseStatement(tokens):
    n_tokens, node = tokens.copy(), {}
    
    return n_tokens, node, False


def parseArguments(tokens):
    n_tokens, node = tokens.copy(), []
    
    if n_tokens.pop().type != 'ROUND_OPEN':
        return tokens, None, True

    token = n_tokens.pop()
    while token.type != 'ROUND_CLOSE':
        n_tokens, expr, error = parseExpression(n_tokens)
        node.append(expr)
        token = n_tokens.pop()

        if error or token.type == 'ENDOFFILE':
            return tokens, None, True
    
    return n_tokens, node, False

    

def parseFunction(tokens):
    n_tokens, node = tokens.copy(), {'type': 'function'}

    keyword_token = n_tokens.pop()
    name_token = n_tokens.pop()
    if keyword_token.type != 'FUN' or name_token.type != 'IDENTIFIER':
        return tokens, None, True
    node['name'] = name_token.value

    n_tokens, node['args'], error = parseArguments(n_tokens)

    bodyTokens = []
    curly_bal = 0

    return n_tokens, node, False


def parseStart(tokens):
    if not tokens:
        return tokens, {}

    tokens, n_node, error = parseFunction(tokens)
    # tokens, n_node, error = parseStatement(tokens)

    return tokens, n_node


with open('test.p1') as file:
    string = file.read()

tokens = lexer.lex(string)[::-1]    # reverse such that pop removes first token

prog = []
do = True
while do:
    tokens, node = parseStart(tokens)
    do = False
    if node:
        prog.append(node)
        do = True
print(prog)