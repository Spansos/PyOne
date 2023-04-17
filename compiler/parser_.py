from lexer import Token


class Error:
    def __init__(self, message: str, bad_token: Token):
        self.message = message
        self.token = bad_token
    
    def __repr__(self):
        # calc on which line
        lines = self.token.program[:self.token.end].split('\n')
        line_num = len(lines)
        # calc where in that line
        prev_lines_len = sum([len(line) for line in lines[:line_num]])
        start = self.token.start-prev_lines_len
        # make and return string
        return f"{self.message} @ line {line_num}, char {start}"


def parseExpression(tokens):
    new_tokens, node = tokens.copy(), dict(type='ASSIGNMENT')
    return new_tokens, node


def parseAssignment(tokens):
    new_tokens, node = tokens.copy(), dict(type='ASSIGNMENT')
    
    new_tokens, left = parseExpression(new_tokens)
    if isinstance(left, Error):
        return tokens, left
    if not left.get('writable'):
        return tokens, Error("Left-hand side of assignment is not writable", new_tokens[-1])
    node['left'] = left

    as_token = new_tokens.pop()
    if as_token.type != 'ASSIGNMENT':
        return tokens, Error("Excpected assignment operator", as_token)
    
    new_tokens, right = parseExpression(new_tokens)
    if isinstance(right, Error):
        return tokens, right
    node['right'] = right

    sc_token = new_tokens.pop()
    if sc_token.type != 'SEMI-COLON':
        return tokens, Error("Excpected semi-colon", sc_token)

    return new_tokens, node


def parseStatement(tokens):
    new_tokens, node = tokens.copy(), dict(type='STATEMENT')

    new_tokens, node = parseAssignment(new_tokens)
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
            return node
    
    return prog


from lexer import lex

with open('test.p1') as file:
    print(parseProgram(lex(file.read())))