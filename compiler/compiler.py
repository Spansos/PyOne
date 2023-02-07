import lexer
import parser_
import stdlib

def generate(node):
    string = ''
    match node['type']:
        case 'body':
            sub_nodes = [generate(sub_node) for sub_node in node['body']]
            string = f"({' or '.join(sub_nodes)})"
        
        case 'statement':
            statement = generate(node['statement'])
            if node['statement']['type'] in ():
                string = f"{statement}"
            else:
                string = f"({statement}, None)[1]"
        
        case 'function_def':
            argstr = ','.join([generate(arg) for arg in node['args']])
            funcstr = f"(lambda {argstr}:{generate(node['body'])})"
            string = f"(({node['name']}:={funcstr}), None)[1]"
        
        case 'function_call':
            args = [generate(arg) for arg in node['args']]
            string = f"{node['id']}({','.join(args)})"

        case 'return':
            string = f"(_return:={generate(node['value'])}, True)[1]"
        
        case 'while':
            body = generate(node['body'])
            expr = generate(node['expr'])
            string = f"(_loop:=[0],_break:=False,[_loop.append({body}) for _ in _loop if{expr}and(not _break)])"
        
        case 'for':
            body = generate(node['body'])
            init = generate(node['init'])
            condition = generate(node['condition'])
            repeat = generate(node['repeat'])
            string = f"(_loop:=[0],_break:=False,_return:=False,{init},[_loop.append(({body},({repeat} if((not _break)and(not _return)) else ()))) for _ in _loop if{condition}and(not _break)])"
        
        case 'break':
            string = "(_break:=True)"
        
        case 'skip':
            string = "(True)"
        
        case 'if':
            expr = generate(node['expr'])
            body = generate(node['body'])
            else_body = generate(node.get('else_body', {'type':'body', 'body':[]}))
            string = f"({body}if{expr}else{else_body})"

        case 'assignment':
            target = generate(node['target'])
            value = generate(node['value'])
            string = f"({target}:={value})"

        case 'table':
            keyvals = [(generate(key), generate(value)) for key, value in node['key-values']]
            keyvalstrs = [f"{key}:{value}" for key, value in keyvals]
            string = f"{{{','.join(keyvalstrs)}}}"
        
        case 'index':
            table = generate(node['table'])
            index = generate(node['index'])
            string = f"(({table})[{index}])"
        
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
code = f"({stdlib.stdlib},{generate(ast)})"

with open('out.py', 'w') as file:
    file.write(code)