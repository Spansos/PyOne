linepath = [[0], ['main', 2]]
prog = (({}, None), ())


run = lambda path, prog: prog[1][path[-1][0]]() if isinstance(path[-1][0], int) else run([*path[:-1], path[-1][1:]], prog[0][0][path[-1][0]])

print(run(linepath, prog))