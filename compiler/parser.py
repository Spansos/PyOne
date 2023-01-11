import lexer

with open('test.p1') as file:
    string = file.read()

tokens = lexer.lex(string)
[print(i) for i in tokens]