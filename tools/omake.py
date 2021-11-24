'''
Author: Dizzrt
Date: 2021-11-15 17:18:13
LastEditTime: 2021-11-23 15:38:21
LastEditors: Dizzrt
FilePath: \Big OS\tools\omake.py
Description: 
'''

import json
import sys
import os
import getopt

CC = 'x86_64-elf-gcc '
CXX = 'x86_64-elf-g++ '
AS = 'x86_64-elf-as '
LD = 'x86_64-elf-ld '
OBJCOPY = 'x86_64-elf-objcopy '
OBJDUMP = 'x86_64-elf-objdump '

Path_src = []
out = 'os.img'
Path_pwd: str
Path_temp: str
configure: json

dump = False
save = False
deg = False


def _deg():
    global Path_pwd
    lock = os.path.join(os.path.join(Path_pwd, 'build'), 'os.img.lock')
    if os.path.exists(lock):
        os.remove(lock)
    os.system(configure['deg'])


def error(msg: str):
    print('\033[31m'+msg+'\033[0m')
    exit(1)


def do(order: str, line: int):
    code = os.system(order)
    if code != 0:
        error('os error at line '+str(line)+'[error code:'+str(code)+']')
    return None


def searchDir(path, rlist: list):
    files = os.listdir(path)
    for file in files:
        curPath = os.path.join(path, file)
        if os.path.isdir(curPath):
            rlist.append(curPath)
            searchDir(curPath, rlist)
    return None


def findSrc(target: str):
    for spath in Path_src:
        temp = os.path.join(spath, target)
        if os.path.exists(temp):
            return temp
    return None


def findSrcs():
    rlist = []
    for file in configure["SRC"]:
        ret = findSrc(file)
        if ret == None:
            error('file:'+file+' not found')
        else:
            rlist.append(ret)
    return rlist


def init():
    global out
    global Path_temp
    Path_temp = os.path.join(Path_pwd, 'build\\temp')
    if not os.path.exists(Path_temp):
        os.makedirs(Path_temp)

    global Path_src
    for i in configure['SRCDIR']:
        temp = os.path.join(Path_pwd, os.path.normcase(i))
        temp = os.path.normpath(temp)

        base = os.path.basename(temp)
        if base == '**':
            temp = os.path.dirname(temp)
            Path_src.append(temp)
            searchDir(temp, Path_src)
        else:
            Path_src.append(temp)
    out = os.path.join(os.path.dirname(Path_temp), out)
    return None


def compile_mbr():
    mbr = '"'+findSrc(configure['mbr'])+'"'
    mbr_o = '"'+os.path.join(Path_temp, 'mbr.o')+'"'
    order = AS+mbr+' '+configure['CP_mbr'] + ' -o '+mbr_o
    do(order, sys._getframe().f_lineno)

    target = '"'+os.path.join(Path_temp, 'mbr')+'"'
    order = LD+mbr_o+' '+configure['LDP_mbr'] + ' -o ' + target
    do(order, sys._getframe().f_lineno)

    if dump:
        objdump(target, 'objdump')
    objcopy(target, 'objcopy_loader')

    return target[1:-1]


def compile_boot():
    boot = '"'+findSrc(configure['boot'])+'"'
    boot_o = '"'+os.path.join(Path_temp, 'boot.o')+'"'
    order = AS+boot+' '+configure['CP_boot'] + ' -o '+boot_o
    do(order, sys._getframe().f_lineno)

    target = '"'+os.path.join(Path_temp, 'boot')+'"'
    order = LD+boot_o+' '+configure['LDP_boot'] + ' -o ' + target
    do(order, sys._getframe().f_lineno)
    if dump:
        objdump(target, 'objdump')
    objcopy(target, 'objcopy_loader')

    return target[1: -1]


def ld_kernel(files: list):
    order = LD
    target = '"'+os.path.join(Path_temp, 'kernel')+'"'
    for file in files:
        order += '"'+file+'" '
    order += '-o '+target+' '

    for i in configure['LDP']:
        order += i+' '
    do(order, sys._getframe().f_lineno)
    if dump:
        objdump(target, 'objdump')
    objcopy(target, 'objcopy_kernel')
    return target[1: -1]


def compile_kernel():
    bins = []
    srcs = findSrcs()
    for src in srcs:
        suffix = os.path.splitext(src)[-1]
        target = os.path.join(Path_temp, str(os.path.basename(src))+'.o')

        order: str
        bins.append(target)
        if suffix == '.cpp':
            order = CXX+'"' + src+'" -o "'+target+'" '
            for param in configure['CP_cxx']:
                order += param+' '
        elif suffix == '.s':
            order = AS+'"' + src+'" -o "'+target+'" '
            for param in configure['CP_asm']:
                order += param+' '
        do(order, sys._getframe().f_lineno)
    return ld_kernel(bins)


def objcopy(file: str, params: str):
    order = OBJCOPY
    for i in configure[params]:
        order += i+' '
    order += file+' '+file
    do(order, sys._getframe().f_lineno)
    return None


def objdump(file: str, params: str):
    order = OBJDUMP
    for i in configure[params]:
        order += i+' '
    order += file+' > ' + \
        '"'+os.path.join(os.path.dirname(Path_temp),
                         (os.path.basename(file).split('.')[0])[:-1]+'.s"')
    do(order, sys._getframe().f_lineno)
    return None


def build():
    mbr = compile_mbr()
    boot = compile_boot()
    kernel = compile_kernel()

    zero = b'\x00'
    buffer: bytes
    with open(mbr, 'rb') as f:
        buffer = f.read()

    with open(boot, 'rb') as f:
        temp = f.read()
        size = os.path.getsize(boot)
        size = size % 512
        if size > 0:
            temp += (zero*(512-size-8))
        else:
            temp += (zero*504)
        temp += b'\x00\x64\x61\x6f\x4c\x64\x6e\x45'
        buffer += temp

    with open(kernel, 'rb') as f:
        temp = f.read()
        size = os.path.getsize(kernel)
        size = size % 512
        if size > 0:
            temp += (zero*(512-size-8))
        else:
            temp += (zero*504)
        temp += b'\x00\x64\x61\x6f\x4c\x64\x6e\x45'
        buffer += temp

    blen = int(len(buffer)/512)
    if blen > 10240:
        error('5Mb space is exceeded')

    if not os.path.exists(out):
        block = bytes()
        for _ in range(64):
            block += b'\x00\x00\x00\x00\x00\x00\x00\x00'

        blen = 10240-blen
        for _ in range(blen):
            buffer += block
        f = open(out, 'wb')
        f.close()

    with open(out, 'rb+') as f:
        f.write(buffer)

    print('\033[32mdone!\033[0m')


def main(args):
    global Path_pwd
    Path_pwd = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

    global configure
    temp = open(Path_pwd+'\\tools\\omake.json', 'r')
    configure = json.load(temp)
    temp.close

    global out
    global dump
    global save
    global deg
    _opts, _args = getopt.gnu_getopt(args, 'o:Dsd', ['debug', 'cls'])
    for opt, arg in _opts:
        if opt == '-o':
            out = arg
        elif opt == '-D':
            dump = True
        elif opt == '-s':
            save = True
        elif opt == '-d':
            deg = True
        elif opt == '--debug':
            _deg()
            return 0
        elif opt == '--cls':
            bp = os.path.join(Path_pwd, 'build')
            flist = os.listdir(bp)
            for i in flist:
                temp = os.path.join(bp, i)
                os.remove(temp)
            os.rmdir(bp)
            return 0
        else:
            error('invalid param: '+opt)

    init()
    build()

    if not save:
        flist = os.listdir(Path_temp)
        for i in flist:
            temp = os.path.join(Path_temp, i)
            os.remove(temp)
        os.rmdir(Path_temp)

    if deg:
        _deg()

    return None


if __name__ == "__main__":
    main(sys.argv[1:])
