stdlib = ('('
'___print:=print,'
'___size:=len,'
'___copy:=dict.copy,'
'___delete:=dict.pop,'
'___get_keys:=lambda _dict:{i:k for i,k in enumerate(_dict.keys())},'
'___get_keys:=lambda _dict:{i:k for i,k in enumerate(_dict.values())},'
'___append:=lambda _dict,_item:(dict.__setitem__(_dict,len(_dict),_item))'
')')