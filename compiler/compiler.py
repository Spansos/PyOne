import lexer
import parser_

with open('test.p1', 'r') as file:
    input = file.read()
tokens = lexer.lex(input)
ast = parser_.parse(tokens)

print(ast)