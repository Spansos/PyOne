from lexer import Token


class Error:
    def __init__(self, message: str, bad_token: Token = None):
        self.message = message
        self.token = bad_token
    
    def __repr__(self):
        if not self.token:
            return f"{self.message} @ somewhere"
        # calc on which line
        lines = self.token.program[:self.token.end].split('\n')
        line_num = len(lines)
        # calc where in that line
        prev_lines_len = sum([len(line) for line in lines[:line_num-1]]) + line_num - 1
        start = self.token.start-prev_lines_len
        # make and return string
        return f"{self.message} @ line {line_num}, char {start}"


def parseInteger(tokens):
    new_tokens, node = tokens.copy(), dict(type='INTEGER')
    token = new_tokens.pop()

    if token.type != "INTEGER":
        return tokens, Error("Invalid integer", token)
    
    node['value'] = token.value
    return new_tokens, node


def parseVariable(tokens):
    new_tokens, node = tokens.copy(), dict(type='IDENTIFIER')
    token = new_tokens.pop()

    if token.type != "IDENTIFIER":
        return tokens, Error("Invalid identifier", token)
    
    node['value'] = token.value
    
    return new_tokens, node
    

def parseExpression(tokens):
    new_tokens = tokens.copy()

    match new_tokens[-1].type:
        case "IDENTIFIER":
            new_tokens, node = parseVariable(new_tokens)
        case "INTEGER":
            new_tokens, node = parseInteger(new_tokens)
        case _:
            return tokens, Error("Invalid Expression", tokens[-1])
    
    while new_tokens and new_tokens[-1].type == 'ROUND_OPEN':
        new_tokens.pop()
        node = dict(type='FUNCALL', function=node, args=[])

        while new_tokens and new_tokens[-1].type != 'ROUND_CLOSE':
            new_tokens, new_arg = parseExpression(new_tokens)
            if isinstance(new_arg, Error): return tokens, new_arg
            node['args'].append(new_arg)

            if not new_tokens: return tokens, Error("Expected closing bracket or comma, but reached end of file")
            if new_tokens and new_tokens[-1].type == 'COMMA':
                new_tokens.pop()
            elif new_tokens and new_tokens[-1].type != 'ROUND_CLOSE':
                return tokens, Error("Expected closing bracket or comma", new_tokens[-1])
        new_tokens.pop()
            

    operator_precedence  = dict(
        **{i:0 for i in ('AND', 'OR', )},
        **{i:1 for i in ('EQUALS', 'NOT_EQUALS', 'GREATER_EQUAL', 'LESSER_EQUAL', 'GREATER', 'LESSER', )},
        **{i:2 for i in ('PLUS', 'MINUS', )},
        **{i:3 for i in ('MULT', 'DIVISION', 'INT_DIVISION', 'MODULUS', )},
        **{i:4 for i in ('POWER', )}
    )
    
    def insert_node(done, new):
        if operator_precedence.get(new['type'], 5) >= operator_precedence.get(done['type'], 5):
            return dict(type=done['type'], lhs=insert_node(done['lhs'], new), rhs=done['rhs'])
        else:
            return dict(type=new['type'], lhs=new['lhs'], rhs=done)

    if new_tokens and new_tokens[-1].type in operator_precedence:
        op_token = new_tokens.pop()

        node = dict(type=op_token.type, lhs=node)
        new_tokens, new_node = parseExpression(new_tokens)

        node = insert_node(new_node, node)
    
    node['writable'] = True
    return new_tokens, node


def parseExpressionStatement(tokens):
    new_tokens = tokens.copy()
    new_tokens, node = parseExpression(new_tokens)

    if isinstance(node, Error): return tokens, node
    if not new_tokens: return tokens, Error("Reached end of file", tokens[0])
    sc_token = new_tokens.pop()
    if sc_token.type != "SEMICOLON": return tokens, Error("Expected semi-colon", sc_token)

    return new_tokens, node


def parseFor(tokens):
    new_tokens, node = tokens.copy(), dict(type='FOR')

    token = new_tokens.pop()
    if token.type != "FOR":
        return tokens, Error("Missing for keyword", token)
    
    new_tokens, node["start"]  = parseStatement(new_tokens)
    new_tokens, node["expr"]   = parseExpressionStatement(new_tokens)
    new_tokens, node["repeat"] = parseStatement(new_tokens)
    new_tokens, node["body"]   = parseStatement(new_tokens)

    if isinstance(node["start"],  Error): return tokens, node["start"]
    if isinstance(node["expr"],   Error): return tokens, node["expr"]
    if isinstance(node["repeat"], Error): return tokens, node["repeat"]
    if isinstance(node["body"],   Error): return tokens, node["body"]
    
    return new_tokens, node


def parseAssignment(tokens):
    new_tokens, node = tokens.copy(), dict(type='ASSIGNMENT')
    
    new_tokens, left = parseExpression(new_tokens)
    if isinstance(left, Error): return tokens, left
    if not left.get('writable'): return tokens, Error("Left-hand side of assignment is not writable", new_tokens[-1])

    as_token = new_tokens.pop()
    if as_token.type != 'ASSIGNMENT': return tokens, Error("Excpected assignment operator", as_token)

    new_tokens, right = parseExpression(new_tokens)
    if isinstance(right, Error): return tokens, right

    sc_token = new_tokens.pop()
    if sc_token.type != 'SEMICOLON': return tokens, Error("Excpected semi-colon", sc_token)

    node['left'] = left
    node['right'] = right

    return new_tokens, node


def parseStatement(tokens):
    new_tokens, node = tokens.copy(), dict(type='STATEMENT')

    match new_tokens[-1].type:
        case "FOR":
            new_tokens, node = parseFor(new_tokens)
        case _:
            new_tokens, node = parseAssignment(new_tokens)
            if isinstance(node, Error):
                new_tokens, node = parseExpressionStatement(new_tokens)

    if isinstance(node, Error):
        return tokens, node

    return new_tokens, node


def parseProgram(tokens) -> dict | Error:
    tokens = tokens[::-1]
    
    prog = dict(type='PROGRAM', body=[])
    while tokens:
        tokens, node = parseStatement(tokens)
        prog['body'].append(node)
        if isinstance(node, Error):
            print(prog)
            print(node.token)
            return node
    
    return prog


from lexer import lex
import json

with open('test.p1') as file:
    print(parseProgram(lex(file.read())))
    # print(json.dumps(parseProgram(lex(file.read())), indent=4))