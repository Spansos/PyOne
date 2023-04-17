from lexer import Token

class Node:
    def __init__(self, type: str, sub_nodes: list['Node']=None):
        self.type = type
        self.sub_nodes = sub_nodes



class Error:
    def __init__(self, message: str, bad_token: Token):
        self.message = message
        self.token = bad_token
    
    def __repr__(self):
        # calc on which line
        lines = self.token.program.split('\n')
        line_num = len(lines)
        # calc where in that line
        prev_lines_len = sum([len(line) for line in lines[:line_num]])
        start = self.token.start-prev_lines_len
        # make and return string
        return f"{self.message} @ line {line_num}, char {start}"


def parseTable(tokens):
    n_tokens, node = tokens.copy(), {'type': 'table', 'key-values': []}
    return n_tokens, node, False

def parseFunCall(tokens):
    n_tokens, node = tokens.copy(), {}
    return n_tokens, node, False



def parseVar(tokens):
    n_tokens, node = tokens.copy(), {}
    return n_tokens, node, False



def parseIndex(tokens):
    n_tokens, node = tokens.copy(), {}
    return n_tokens, node, False



def parseExpression(tokens):
    n_tokens, node = tokens.copy(), {'type':'expression'}
    return n_tokens, node, False


def parseBody(tokens):
    n_tokens, node = tokens.copy(), {'type': 'body', 'body':[]}
    return n_tokens, node, False


def parseIf(tokens):
    n_tokens, node = tokens.copy(), {'type': 'if'}
    return n_tokens, node, False

def parseReturn(tokens):
    n_tokens, node = tokens.copy(), {'type': 'return'}
    return n_tokens, node, False

def parseWhile(tokens):
    n_tokens, node = tokens.copy(), {'type': 'while'}
    return n_tokens, node, False

def parseFor(tokens):
    n_tokens, node = tokens.copy(), {'type': 'for'}
    return n_tokens, node, False

def parseSkip(tokens):
    n_tokens, node = tokens.copy(), {'type': 'skip'}
    return n_tokens, node, False

def parseBreak(tokens):
    n_tokens, node = tokens.copy(), {'type': 'break'}
    return n_tokens, node, False

def parseStatement(tokens):
    n_tokens, node = tokens.copy(), {'type': 'statement'}
    return n_tokens, node, False


def parseFunDef(tokens):
    n_tokens, node = tokens.copy(), {'type': 'function_def'}
    return n_tokens, node, False


def parseProgram(tokens):
    tokens = tokens[::-1]
    
    prog = {'type': 'body', 'body': []}
    while True:
        tokens, node, error = parseStatement(tokens)
        if error == 'END':
            break
        if error:
            print(f'ERROR:\n\tprog:\t{prog}\n\ttokens:\t{tokens[::-1]}')
            break
        prog['body'].append(node)
    
    return prog


from lexer import lex
import json

with open('test.p1') as file:
    print(json.dumps(parseProgram(lex(file.read())), indent=1))