stdlib = ('('
# prints things. needs stricter formatting and table needs better formatting
'___print:='
    'lambda _item:'
        'print(_item),'

# num of items in table or string
'___size:='
    'lambda _dict:'
        'len(_dict),'

# creates copy of table
'___copy:='
    'dict.copy,'

# removes key from table
'___remove:='
    'dict.pop,'

# removes index from list, shifts later items to maintain list
'___pop:='
    'lambda _dict,_index:'
        '('
            '_dict[_index],'
            '[_dict.__setitem__(_i,_dict[_i+1]) for _i in range(_index,___size(_dict)-1)],'
            '___remove(_dict,___size(_dict)-1)'
        ')[0],'

# gives list of table key (list is table with keys of 0,1,2...)
'___get_keys:='
    'lambda _dict:'
        '{i:k for i,k in enumerate(_dict.keys())},'

# appends value to list. will be a shit if _dict is not list
'___append:='
    'lambda _dict,_item:'
        '(dict.__setitem__(_dict,len(_dict),_item))'

')')