# linepath = [[0], ['main', 2]]
# prog = (({}, None), ())


# run = lambda path, prog: prog[1][path[-1][0]]() if isinstance(path[-1][0], int) else run([*path[:-1], path[-1][1:]], prog[0][0][path[-1][0]])

# print(run(linepath, prog))

# import timeit

# lst = [1]
# [lst.append(1) for i in lst if len(lst) < 1000]
# print(len(lst))

# test()

# (lambda: (
#     (c := 2),
#     print(c),
#     (c := c + 1),
#     (print(c)),
#     )
# )()

tokens = ['val0', 'op3', 'val1', 'op1', 'val2', 'op2', 'val3', 0]
for op in ('op1', 'op2', 'op3'):
    i = 0
    while i < len(tokens):
        if tokens[i] == op:
            tokens[i-1] = {'op': op, 'rhs': tokens.pop(i+1), 'lhs': tokens.pop(i-1)}
        else:
            i += 1

print(tokens)