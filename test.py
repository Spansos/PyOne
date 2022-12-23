a, r = [0], lambda: (print(a.__getitem__(0)), a.__setitem__(0, a.__getitem__(0)+1), print(a.__getitem__(0)), print())
r()
r()