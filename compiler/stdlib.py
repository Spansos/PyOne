stdlib = ('('
'___print:=print,'                                                          # prints value. table needs better format
'___size:=len,'                                                             # num of items in table
'___copy:=dict.copy,'                                                       # creates copy of table
'___remove:=dict.pop,'                                                      # removes key from table
'___get_keys:=lambda _dict:{i:k for i,k in enumerate(_dict.keys())},'       # gives list of table key (list is table with keys of 0,1,2...)
'___get_values:=lambda _dict:{i:k for i,k in enumerate(_dict.values())},'   # gives list of table values
'___append:=lambda _dict,_item:(dict.__setitem__(_dict,len(_dict),_item))'  # appends value to list. will be weird if _dict is not list
')')