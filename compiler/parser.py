import lexer

def parseTable(tokens):
    n_tokens, node = tokens.copy(), []
    return n_tokens, node, False

def parseFunCall(tokens):
    n_tokens, node = tokens.copy(), {}

    node['type'] = 'function_call'
    node['id']   =  n_tokens.pop().value
    n_tokens, node['args'], error = parseArguments(n_tokens)

    if error:
        return tokens, None, True

    return n_tokens, node, False

def parseVar(tokens):
    n_tokens, node = tokens.copy(), {}

    token = n_tokens.pop()
    if token.type != 'IDENTIFIER':
        return tokens, None, True

    node['type'] = 'var'
    node['id']   = token.value

    return n_tokens, node, False

def parseIndex(tokens):
    n_tokens, node = tokens.copy(), {}

    if n_tokens.pop().type != 'SQUARE_OPEN':
        return tokens, None, True

    node['type'] = 'index'
    node['id']   = n_tokens.pop()
    node['index']= parseExpression(n_tokens)
    
    if n_tokens.pop().type != 'SQUARE_CLOSE':
        return tokens, None, True

    return n_tokens, node, False

def parseExpression(tokens):
    n_tokens, node = tokens.copy(), {}
    node['type'] = 'expression'
    
    sub_nodes = []
    while n_tokens[-1].type != 'ENDOFFILE':
        match n_tokens[-1].type:
            case 'CURLY_OPEN':
                n_tokens, sub_node, error = parseTable(n_tokens)
                if error:
                    return tokens, None, False
                sub_nodes.append(sub_node)

            case 'IDENTIFIER':
                n_tokens, sub_node, error = parseFunCall(n_tokens)
                if error:
                    n_tokens, sub_node, error = parseIndex(n_tokens)
                if error:
                    n_tokens, sub_node, error = parseVar(n_tokens)
                if error:
                    return tokens, None, True
                sub_nodes.append(sub_node)
            
            case 'INTEGER'|'FLOAT'|'STRING'|'BOOL':
                sub_node = {}
                sub_node['type'] = 'literal'
                sub_node['datatype'] = n_tokens[-1].type
                sub_node['value'] = n_tokens[-1].value

            case 'NOT'|'EQUALS'|'NOT_EQUALS'|'GREATER_EQUALS'|'LESSER_EQUALS'|'GREATER'|'LESSER'|'AND'|'OR'|'PLUS'|'MULT'|'POWER'|'DIVISION'|'INT_DIVISION'|'MODULUS':
                sub_nodes.append(n_tokens.pop())

            case _:
                if not sub_nodes:
                    return tokens, None, True
                break

    for node in sub_nodes:
        print(node)
        if not isinstance(node, dict) and node.type == 'NOT':
            pass
    
    return n_tokens, node, False



def parseStatement(tokens):
    n_tokens, node = tokens.copy(), {}
    
    return n_tokens, node, False
    

def parseFunDef(tokens):
    n_tokens, node = tokens.copy(), {'type': 'function_def'}

    keyword_token = n_tokens.pop()
    name_token = n_tokens.pop()
    if keyword_token.type != 'FUN' or name_token.type != 'IDENTIFIER':
        return tokens, None, True
    node['name'] = name_token.value

    # print(n_tokens)
    node['args'] = []
    if n_tokens.pop().type != 'ROUND_OPEN':
        return tokens, None, True

    while n_tokens[-1].type != 'ROUND_CLOSE':
        n_tokens, var, error = parseVar(n_tokens)

        node['args'].append(var)

        if n_tokens[-1].type == 'COMMA':
            n_tokens.pop()
        elif n_tokens[-1].type != 'ROUND_CLOSE':
            return tokens, None, True

        if error or n_tokens[-1].type == 'ENDOFFILE':
            return tokens, None, True

    bodyTokens = []
    curly_bal = 0

    return n_tokens, node, False


def parseStart(tokens):
    if not tokens:
        return tokens, {}

    tokens, n_node, error = parseFunDef(tokens)
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