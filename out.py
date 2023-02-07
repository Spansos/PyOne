(
    (___print:=print,___size:=len,___copy:=dict.copy,___delete:=dict.pop,___get_keys:=lambda _dict:{i:k for i,k in enumerate(_dict.keys())},___get_keys:=lambda _dict:{i:k for i,k in enumerate(_dict.values())},___append:=lambda _dict,_item:(dict.__setitem__(_dict,len(_dict),_item))),
    (
    ((_return := (False, None)), None)[1] or
    ((___main:=(
        lambda :(
            ((___nums:={0:1,1:1}), None)[1] or
            ((_loop:=[0],_break:=False,((___i:=0), None)[1],[_loop.append((
                (((___size_:=___size(___nums)), None)[1] or
                (___append(___nums,(((___nums)[(___size_-1)])+((___nums)[(___size_-2)]))), None)[1]),
                (((___i:=(___i+1)), None)[1] ))) for _ in _loop if(___i<10)and(not _break)and(not _return[0])]),_return if _return[0] else None) or
            (___print(___nums), None)[1])
            )), None)[1] or
        (___main(), None)[1]
    )
)