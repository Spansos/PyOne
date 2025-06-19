L = type('',(),{'s':0,'__call__':lambda s:setattr(s,'s',1),'__next__':lambda s:iter([]).__next__() if s.s else s,'__iter__':lambda s:s})
R = type('',(),{'__bool__':lambda s:False,'__getitem__':lambda s,i:s})
r = R()
b = 0
___print = print

((___main:=lambda:(r:=R(),b:=0,((((___c:=(0)),r or b)[1]or(({(((((___c:=((___c)+(1))),r or b)[1]or(((r:=((___c),))if((___c)==(10))else 0),r or b)[1]or(((___print)((___c),)),r or b)[1]),r or b)[1],l()if b or r or not((___c)<(20))else 0)[1]for l in L()},r,b:=0)[1],r or b)[1]),r or b)[1])[2][0]),r or b)[1]or(((___print)(((___main)()),)),r or b)[1]