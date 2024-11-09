import dis, marshal, struct, sys, time, types
 
def show_file(fname):
    f = open(fname, "rb")
    magic = f.read(4)
    moddate = f.read(4)
    #modtime = time.asctime(time.localtime(struct.unpack('L', moddate)[0]))
    print("magic %s" % magic.hex())
    #print "moddate %s (%s)" % (moddate.encode('hex'), modtime)

    cdate = f.read(4)
    fsize = f.read(4)

    code = marshal.load(f)
    show_code(code)
     
def show_code(code, indent='', const_idx=None):
    old_indent = indent
    if const_idx:
        print("%s{%d}<code>" % (indent, const_idx))
    else:
        print("%s<code>" % indent)
    indent += '   '
    print("%s<argcount> %d </argcount>" % (indent, code.co_argcount))
    print("%s<nlocals> %d</nlocals>" % (indent, code.co_nlocals))
    print("%s<stacksize> %d</stacksize>" % (indent, code.co_stacksize))
    print("%s<flags> %04x</flags>" % (indent, code.co_flags))
    show_hex("code", code.co_code, indent=indent)
    print("%s<dis>" % indent)
    dis.disassemble(code)
    print("%s</dis>" % indent)
 
    print("%s<names> %r</names>" % (indent, code.co_names))
    print("%s<varnames> %r</varnames>" % (indent, code.co_varnames))
    print("%s<freevars> %r</freevars>" % (indent, code.co_freevars))
    print("%s<cellvars> %r</cellvars>" % (indent, code.co_cellvars))
    print("%s<filename> %r</filename>" % (indent, code.co_filename))
    print("%s<name> %r</name>" % (indent, code.co_name))
    print("%s<firstlineno> %d</firstlineno>" % (indent, code.co_firstlineno))
 
    print("%s<consts>" % indent)
    const_idx = 0
    for const in code.co_consts:
        if type(const) == types.CodeType:
            show_code(const, indent+'   ', const_idx=const_idx)
        else:
            print("   %s{%d}%r" % (indent, const_idx, const))
        const_idx += 1
    print("%s</consts>" % indent)
 
    show_hex("lnotab", code.co_lnotab, indent=indent)
    if const_idx:
        print("%s</code>{%d}" % (old_indent, const_idx))
    else:
        print("%s</code>" % old_indent)
     
def show_hex(label, h, indent):
    h = h.hex()
    if len(h) < 60:
        print("%s<%s> %s</%s>" % (indent, label, h,label))
    else:
        print("%s<%s>" % (indent, label))
        for i in range(0, len(h), 60):
            print("%s   %s" % (indent, h[i:i+60]))
        print("%s</%s>" % (indent, label))
 
show_file(sys.argv[1])
