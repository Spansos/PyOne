((___print:=print,___size:=len,___copy:=dict.copy,___remove:=dict.pop,___get_keys:=lambda _dict:{i:k for i,k in enumerate(_dict.keys())},___get_values:=lambda _dict:{i:k for i,k in enumerate(_dict.values())},___append:=lambda _dict,_item:(dict.__setitem__(_dict,len(_dict),_item))),(((_return:=[False, None]), None)[1] or ((___factorial:=(lambda ___n:((((_return:=[False, None]), None)[1] or ((___print(___n)), None)[1] or ((((_return:=[False, None]), None)[1] or (_return:=(1, True)))if(___n==1)else(((_return:=[False, None]), None)[1])) or (_return:=((___n*(___factorial((___n-1)))), True))),_return[0])[1])),None)[1] or ((___print((___factorial(8)))), None)[1]))