linepath = [['main', 0]]
prog = (({'main': 2}, None), ())

def run(path, prog):
    n_path = path[1:]
    if isinstance(n_path[0], int):
        

print(run(linepath, prog))