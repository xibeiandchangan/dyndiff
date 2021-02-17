
# -*- coding: utf-8 -*-

import os
import json
import struct
 
from idaapi import *
from idautils import *
from idc import *

JOSN=False

def GetFunctionDecl(functionAddr):
    try:
        if not init_hexrays_plugin():
            return False
        cfunc = decompile(functionAddr)
        if cfunc == None:
            return False
        firstline = tag_remove(cfunc.get_pseudocode()[0].line)
        return firstline
    except:
        return False

def GetFunctionArgs(funcstr):
    lBracket = funcstr.find('(')
    rBracket = funcstr.find(')')
    if lBracket != rBracket - 1:
        return len(funcstr[lBracket:rBracket+1].split(','))
    return 0


def main():
    func_list = Functions()
    funclist = []
    print '[*] start'
    for funcAddr in func_list:
        funcDecl = GetFunctionDecl(funcAddr)
        args = 0
        if funcDecl:
            args = GetFunctionArgs(funcDecl)
        else:
            funcDecl = '[!] decompile Fail'
        funclist.append({
            'func addr':hex(funcAddr).replace('L',''), 
            'func name':GetFunctionName(funcAddr), 
            'func pseudo decl':funcDecl,
            'num of arg':args
            })
    print '[*] end'
    if JOSN:
        f = open('./funclist.json','wb')
        f.write(json.dumps(funclist, indent=4, ensure_ascii=False))
        f.close()
    else:
        allfunc = open('./allfunc.bin', 'wb')
        allfunc.write(struct.pack('<L',len(funclist)))
        for func in funclist:
            allfunc.write(struct.pack('<L',int(func['func addr'],16))+struct.pack('<H',func['num of arg']))
        allfunc.close()
if __name__=="__main__":
	main()
