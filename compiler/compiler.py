import lexer
import parser

with open('test.p1', 'r') as file:
    input = file.read()
tokens = lexer.lex(input)
ast = parser.parse(tokens)

print(ast)