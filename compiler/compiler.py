import lexer
import parser_
import stdlib

def generate(node):
    string = ''
    match node['type']:
        case 'body':
            sub_nodes = ['((_return:=[False, None]), None)[1]']
            sub_nodes += [generate(sub_node) for sub_node in node['body']]
            string = f"({' or '.join(sub_nodes)})"
        
        case 'statement':
            statement = generate(node['statement'])
            if node['statement']['type'] in ():
                string = f"{statement}"
            else:
                string = f"({statement}, None)[1]"
        
        case 'function_def':
            argstr = ','.join([generate(arg) for arg in node['args']])
            funcstr = f"(lambda{' ' if argstr else ''}{argstr}:({generate(node['body'])},_return[0])[1])"
            string = f"(({node['name']}:={funcstr}),None)[1]"
        
        case 'function_call':
            args = [generate(arg) for arg in node['args']]
            string = f"({node['id']}({','.join(args)}))"

        case 'return':
            string = f"(_return:=({generate(node['value'])}, True))"
        
        case 'while':
            body = generate(node['body'])
            expr = generate(node['expr'])
            loop = f"(_loop:=[0],_break:=False,[(_loop.append(0),{body}) for _ in _loop if{expr}and(not _break)and(not _return[0])])"
            string = f"({loop},_break:=False,_return if _return[0] else None)[2]"
        
        case 'for':
            body = generate(node['body'])
            init = generate(node['init'])
            condition = generate(node['condition'])
            repeat = generate(node['repeat'])
            loop = f"(_loop:=[0],_break:=False,{init},[(_loop.append(0),{body},({repeat} if((not _break)and(not _return[0])) else None)) for _ in _loop if{condition}and(not _break)and(not _return[0])])"
            string = f"({loop},_break:=False,_return if _return[0] else None)[2]"
        
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
            if node['target']['type'] == 'index':
                id =    generate(node['target']['table'])
                key =   generate(node['target']['index'])
                value = generate(node['value'])
                string = f"(dict.__setitem__({id},{key},{value}))"
            else:
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

        case 'greater_equal':
            lhs = generate(node['lhs'])
            rhs = generate(node['rhs'])
            string = f"({lhs}>={rhs})"

        case 'lesser_equal':
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
        
        case 'float':
            string = node['value']
        
        case 'bool':
            string = generate({'type':'integer', 'value': str(1 if node['value']=='true' else 0)})

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