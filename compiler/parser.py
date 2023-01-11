with open('test.p1') as file:
    string = file.read()

import lexer

tokens = lexer.lex(string)
[print(i) for i in tokens]
