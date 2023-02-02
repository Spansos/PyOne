import lexer
import parser_

def generate(node):
    string = ''
    match node['type']:
        case 'body':
            sub_nodes = [generate(sub_node) for sub_node in node['body']]
            string = f"({' or '.join(sub_nodes)})"
        
        case 'function_def':
            argstr = ','.join([generate(arg) for arg in node['args']])
            funcstr = f"(lambda {argstr}:{generate(node['body'])})"
            string = f"({node['name']}:={funcstr})"
        
        case 'function_call':
            args = [generate(arg) for arg in node['args']]
            string = f"{node['id']}({','.join(args)})"

        case 'return':
            string='"_ret"'
        
        case 'while':
            body = generate(node['body'])
            expr = generate(node['expr'])
            string = f"(_loop:=[0],[(_loop.append(0),{body}) for i in _loop if{expr}])"
        
        case 'for':
            body = generate(node['body'])
            init = generate(node['init'])
            condition = generate(node['condition'])
            repeat = generate(node['repeat'])
            string = f"(_loop:=[0],{init},[(_loop.append(0),{body},{repeat}) for i in _loop if{condition}])"
        
        case 'if':
            expr = generate(node['expr'])
            body = generate(node['body'])
            else_body = generate(node.get('else_body', {'type':'body', 'body':[]}))
            string = f"({body} if {expr} else {else_body})"

        case 'assignment':
            target = generate(node['target'])
            value = generate(node['value'])
            string = f"({target}:=({value}), None)[1]"

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
            string = f"({lhs}=={rhs})"
        
        case 'not_equals':
            lhs = generate(node['lhs'])
            rhs = generate(node['rhs'])
            string = f"({lhs}!={rhs})"

        case 'greater_equals':
            lhs = generate(node['lhs'])
            rhs = generate(node['rhs'])
            string = f"({lhs}>={rhs})"

        case 'lesser_equals':
            lhs = generate(node['lhs'])
            rhs = generate(node['rhs'])
            string = f"({lhs}<={rhs})"

        case 'greater':
            lhs = generate(node['lhs'])
            rhs = generate(node['rhs'])
            string = f"({lhs}>{rhs})"

        case 'lesser':
            lhs = generate(node['lhs'])
            rhs = generate(node['rhs'])
            string = f"({lhs}<{rhs})"
        
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