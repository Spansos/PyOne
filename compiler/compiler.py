import lexer
import parser_

def generate(node):
    string = ''
    match node['type']:
        case 'prog':
            substrings = [generate(node) for node in node['body']]
            string = f"({','.join(substrings)})"
        
        case 'function_call':
            args = [generate(arg) for arg in node['args']]
            string = f"{node['id']}({','.join(args)})"

        case 'assignment':
            target = generate(node['target'])
            value = generate(node['value'])
            string = f"({target}:=({value}))"

        case 'table':
            keyvals = [(generate(key), generate(value)) for key, value in node['key-values']]
            keyvalstrs = [f"{key}:{value}" for key, value in keyvals]
            string = f"{{{','.join(keyvalstrs)}}}"
        
        case 'and':
            lhs = generate(node['lhs'])
            rhs = generate(node['rhs'])
            string = f"({lhs} and {rhs})"

        case 'or':
            lhs = generate(node['lhs'])
            rhs = generate(node['rhs'])
            string = f"({lhs} or {rhs})"
        case 'equals':
            lhs = generate(node['lhs'])
            rhs = generate(node['rhs'])
            string = f"({lhs} == {rhs})"
        
        case 'not_equals':
            lhs = generate(node['lhs'])
            rhs = generate(node['rhs'])
            string = f"({lhs} != {rhs})"

        case 'greater_equals':
            lhs = generate(node['lhs'])
            rhs = generate(node['rhs'])
            string = f"({lhs} >= {rhs})"

        case 'lesser_equals':
            lhs = generate(node['lhs'])
            rhs = generate(node['rhs'])
            string = f"({lhs} <= {rhs})"

        case 'greater':
            lhs = generate(node['lhs'])
            rhs = generate(node['rhs'])
            string = f"({lhs} > {rhs})"

        case 'lesser':
            lhs = generate(node['lhs'])
            rhs = generate(node['rhs'])
            string = f"({lhs} < {rhs})"
        
        case 'not':
            value = generate(node['value'])
            string = f"(not {value})"
        
        case 'power':
            lhs = generate(node['lhs'])
            rhs = generate(node['rhs'])
            string = f"({lhs}**{rhs})"
        
        case 'mult':
            lhs = generate(node['lhs'])
            rhs = generate(node['rhs'])
            string = f"({lhs}*{rhs})"
        
        case 'division':
            lhs = generate(node['lhs'])
            rhs = generate(node['rhs'])
            string = f"({lhs}/{rhs})"

        case 'int_division':
            lhs = generate(node['lhs'])
            rhs = generate(node['rhs'])
            string = f"({lhs}//{rhs})"
        
        case 'modulos':
            lhs = generate(node['lhs'])
            rhs = generate(node['rhs'])
            string = f"({lhs}%{rhs})"

        case 'plus':
            lhs = generate(node['lhs'])
            rhs = generate(node['rhs'])
            string = f"({lhs}+{rhs})"

        case 'minus':
            lhs = generate(node['lhs'])
            rhs = generate(node['rhs'])
            string = f"({lhs}-{rhs})"
        
        case 'var':
            string = node['id']

        case 'string':
            string = node['value']
        
        case 'integer':
            string = node['value']

        case other:
            print(other)
    
    return string

with open('test.p1', 'r') as file:
    input = file.read()
tokens = lexer.lex(input)
ast = parser_.parse(tokens)
print(generate(ast))