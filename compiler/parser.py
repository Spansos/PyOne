import lexer



def parseTable(tokens):
    n_tokens, node = tokens.copy(), {'type': 'table', 'key-values': []}
    
    if n_tokens.pop().type != 'CURLY_OPEN':
        return tokens, None, True

    while True:
        n_tokens, key, error = parseExpression(n_tokens)
        if error: return tokens, None, True
        
        if n_tokens.pop().type != 'COLON':
            return tokens, None, True
        
        n_tokens, value, error = parseExpression(n_tokens)
        if error: return tokens, None, True
        
        node['key-values'].append((key, value))
        
        token = n_tokens.pop()
        if token.type == 'CURLY_CLOSE':
            break
        if token.type != 'COMMA':
            return tokens, None, True
    
    return n_tokens, node, False



def parseFunCall(tokens):
    n_tokens, node = tokens.copy(), {}

    node['type'] = 'function_call'
    node['args'] = []
    node['id']   =  n_tokens.pop().value
    
    if n_tokens.pop().type != 'ROUND_OPEN':
        return tokens, None, True
    
    while n_tokens[-1].type != 'ROUND_CLOSE':
        n_tokens, expr, error = parseExpression(n_tokens)
        if error: return tokens, None, True
        
        node['args'].append(expr)

        if n_tokens[-1].type == 'COMMA':
            n_tokens.pop()
        else:
            break
    if n_tokens.pop().type != 'ROUND_CLOSE':
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
    n_tokens, node['expression'], error   = parseExpression(n_tokens)
    if not error:
        n_tokens, node['index'], error = parseExpression(n_tokens)
    
    if n_tokens.pop().type != 'SQUARE_CLOSE' or error:
        return tokens, None, True

    return n_tokens, node, False



def parseExpression(tokens):
    n_tokens, node = tokens.copy(), {}
    node['type'] = 'expression'
    
    sub_nodes = []
    next_is_val = True
    while n_tokens and n_tokens[-1].type != 'ENDOFFILE':
        match n_tokens[-1].type:
            case 'ROUND_OPEN':
                if not next_is_val:
                    if not sub_nodes:
                        return tokens, None, True
                    break
                n_tokens.pop()

                n_tokens, sub_node, error = parseExpression(n_tokens)
                if error: return tokens, None, False

                if n_tokens.pop().type != 'ROUND_CLOSE':
                    return tokens, None, True

                sub_nodes.append(sub_node)
                next_is_val = False

            case 'CURLY_OPEN':
                if not next_is_val:
                    if not sub_nodes:
                        return tokens, None, True
                    break
                
                n_tokens, sub_node, error = parseTable(n_tokens)
                if error: return tokens, None, False
                
                sub_nodes.append(sub_node)
                next_is_val = False

            case 'IDENTIFIER':
                if not next_is_val:
                    if not sub_nodes:
                        return tokens, None, True
                    break
                n_tokens, sub_node, error = parseFunCall(n_tokens)
                if error:
                    n_tokens, sub_node, error = parseIndex(n_tokens)
                if error:
                    n_tokens, sub_node, error = parseVar(n_tokens)
                if error:
                    return tokens, None, True
                
                sub_nodes.append(sub_node)
                next_is_val = False
            
            case 'INTEGER'|'FLOAT'|'STRING'|'BOOL':
                if not next_is_val:
                    if not sub_nodes:
                        return tokens, None, True
                    break
                token = n_tokens.pop()
                sub_node = {}
                
                sub_node['type'] = 'literal'
                sub_node['datatype'] = token.type
                sub_node['value'] = token.value
                
                sub_nodes.append(sub_node)
                next_is_val = False

            case 'NOT':
                if not next_is_val:
                    if not sub_nodes:
                        return tokens, None, True
                    break
                sub_nodes.append(n_tokens.pop())
                next_is_val = True
                

            case 'EQUALS'|'NOT_EQUALS'|'GREATER_EQUALS'|'LESSER_EQUALS'|'GREATER'|'LESSER'|'AND'|'OR'|'PLUS'|'MINUS'|'MULT'|'POWER'|'DIVISION'|'INT_DIVISION'|'MODULUS':
                if next_is_val:
                    if not sub_nodes:
                        return tokens, None, True
                    break
                sub_nodes.append(n_tokens.pop())
                next_is_val = True

            case _:
                if not sub_nodes:
                    return tokens, None, True
                break
            

    if len(sub_nodes) == 1:
        node = sub_nodes[0]
        return n_tokens, node, False
    
    for i, v in enumerate(sub_nodes):
        if isinstance(sub_nodes[i], lexer.Token) and sub_nodes[i].type == 'NOT':
            sub_nodes[i] = {'op': 'NOT', 'value': sub_nodes.pop(i+1)}


    for ops in (('POWER',), ('MULT', 'DIVISION', 'INT_DIVISION', 'MODULUS'), ('PLUS', 'MINUS'), ('EQUALS', 'NOT_EQUALS', 'GREATER_EQUALS', 'LESSER_EQUALS', 'GREATER', 'LESSER'), ('AND', 'OR')):
        i = 0
        while i < len(sub_nodes):
            if isinstance(sub_nodes[i], lexer.Token) and sub_nodes[i].type in ops:
                if i-1 < 0 or i+1 > len(sub_nodes)-1:
                    return tokens, None, True
                sub_nodes[i-1] = {'op': sub_nodes[i].type, 'rhs': sub_nodes.pop(i+1), 'lhs': sub_nodes.pop(i-1)}
            else:
                i += 1
    
    node = sub_nodes[0]
    
    return n_tokens, node, False


def parseBody(tokens):
    n_tokens, node = tokens.copy(), []

    if n_tokens.pop().type != 'CURLY_OPEN':
        return tokens, None, False

    body_tokens = []

    curly_bal = 1
    while curly_bal != 0:
        match (token := n_tokens.pop()).type:
            case 'CURLY_OPEN':
                curly_bal += 1
                if curly_bal:
                    body_tokens.append(token)
            case 'CURLY_CLOSE':
                curly_bal -= 1
                if curly_bal:
                    body_tokens.append(token)
            case 'ENDOFFILE':
                return tokens, None, True
            case _:
                body_tokens.append(token)
    body_tokens = body_tokens[::-1]
    
    error = False
    while body_tokens and not error:
        body_tokens, body_node, error = parseStart(body_tokens)
        # print(body_tokens, '\n', body_node, '\n', error, '\n')
        node.append(body_node)
    if error:
        return tokens, None, True

    return n_tokens, node, False


def parseIf(tokens):
    n_tokens, node = tokens.copy(), {'type': 'if'}
    
    # print()
    # print(tokens)

    if n_tokens.pop().type != 'IF':
        # print(1)
        return tokens, None, True
    
    n_tokens, node['expr'], error = parseExpression(n_tokens)
    if error:
        # print(2)
        return tokens, None, True 
    n_tokens, node['body'], error = parseBody(n_tokens)
    if error:
        # print(3)
        return tokens, None, True

    if n_tokens and n_tokens[-1].type == 'ELSE':
        n_tokens.pop()
        n_tokens, node['else_body'], error = parseBody(n_tokens)
        if error:
            # print(5)
            return tokens, None, True
    
    # print(6)
    return n_tokens, node, False

def parseReturn(tokens):
    n_tokens, node = tokens.copy(), {'type': 'return'}
    
    if n_tokens.pop().type != 'RETURN':
        return tokens, None, True

    n_tokens, node, error = parseExpression(n_tokens)
    if error or (n_tokens and n_tokens.pop().type != 'SEMICOLON'):
        return tokens, None, True
    
    return n_tokens, node, False

def parseWhile(tokens):
    n_tokens, node = tokens.copy(), {'type': 'while'}
    
    if n_tokens.pop().type != 'WHILE':
        return tokens, None, True
    
    n_tokens, node['expr'], error = parseExpression(n_tokens)
    if error:
        return tokens, None, True
    
    n_tokens, node['body'], error = parseBody(n_tokens)
    if error:
        return tokens, None, True
    
    return n_tokens, node, False

def parseFor(tokens):
    n_tokens, node = tokens.copy(), {'type': 'for'}
    
    if n_tokens.pop().type != 'FOR':
        return tokens, None, True

    n_tokens, node['init'], error = parseStatement(n_tokens)
    if error or n_tokens.pop().type != 'COMMA':
        return tokens, None, True
    n_tokens, node['condition'], error = parseExpression(n_tokens)
    if error or n_tokens.pop().type != 'COMMA':
        return tokens, None, True
    n_tokens, node['repeat'], error = parseStatement(n_tokens)
    if error:
        return tokens, None, True
    
    n_tokens, node['body'], error = parseBody(n_tokens)
    if error: return tokens, None, True
    
    return n_tokens, node, False
    

def parseStatement(tokens):
    n_tokens, node = tokens.copy(), {}
    
    n_tokens, lhs, error = parseIndex(n_tokens)
    if error:
        n_tokens, lhs, error = parseVar(n_tokens)
    if (not error) and (n_tokens.pop().type == 'ASSIGNMENT'):
        n_tokens, rhs, error = parseExpression(n_tokens)
        node = {'type': 'assignment', 'target': lhs, 'value': rhs}
    else:
        n_tokens = tokens.copy()
        n_tokens, node, error = parseExpression(n_tokens)
    
    if error:
        return tokens, None, True
    
    return n_tokens, node, False


def parseLine(tokens):
    n_tokens, node = tokens.copy(), {}
    
    n_tokens, node, error = parseStatement(n_tokens)
    
    if n_tokens.pop().type != 'SEMICOLON':
        error = True
        
    if error: return tokens, None, True
    
    return n_tokens, node, False
    


def parseFunDef(tokens):
    n_tokens, node = tokens.copy(), {'type': 'function_def'}

    keyword_token = n_tokens.pop()
    name_token = n_tokens.pop()
    
    if keyword_token.type != 'FUN' or name_token.type != 'IDENTIFIER':
        return tokens, None, True
    
    node['name'] = name_token.value
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
    n_tokens.pop()
    
    n_tokens, node['body'], error = parseBody(n_tokens)
    
    return n_tokens, node, False



def parseStart(tokens):
    if not tokens or tokens[-1].type == 'ENDOFFILE':
        return tokens, None, 'END'

    n_tokens, n_node, error = parseIf(tokens)
    if error:
        n_tokens, n_node, error = parseWhile(tokens)
    if error:
        n_tokens, n_node, error = parseFor(tokens)
    if error:
        n_tokens, n_node, error = parseReturn(tokens)
    if error:
        n_tokens, n_node, error = parseFunDef(tokens)
    if error:
        n_tokens, n_node, error = parseLine(tokens)
    if error:
        return tokens, None, True

    return n_tokens, n_node, False

def parse(tokens):
    tokens = tokens[::-1]
    
    prog = []
    while True:
        tokens, node, error = parseStart(tokens)
        if error == 'END':
            break
        if error:
            print(f'ERROR:\n\tafter:\t{prog[-1]}\n\ttokens:\t{tokens[::-1]}')
            break
        prog.append(node)
    
    return prog