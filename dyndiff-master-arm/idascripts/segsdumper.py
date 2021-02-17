import idautils
import idaapi
import struct
#
# name    0x0
# start   0x20
# end     0x28
# perm    0x30
# size    0x38
# cont    0x40

allsegsfd = open("dumpsegs.bin", "wb")
segcount = 0
for ea in idautils.Segments():
    segcount += 1
allsegsfd.write(struct.pack("<L", segcount))
for ea in idautils.Segments():
    segstr = ''
    seg = idaapi.getseg(ea)
    segname = idc.SegName(ea)
    segstart = idc.SegStart(ea)
    segend = idc.SegEnd(ea)
    segperm = GetSegmentAttr(ea, SEGATTR_PERM)
    segstr += segname.ljust(0x20, '\x00')
    segstr += struct.pack("<L", segstart)
    segstr += struct.pack("<L", segend)
    segstr += struct.pack("<L", segperm)
    tcont = ''
    for addr in xrange(segstart, segend):
        tcont += chr(Byte(addr))
    segstr += struct.pack("<L", len(tcont))
    segstr += tcont
    allsegsfd.write(segstr)
allsegsfd.close()
print("segments dump has been written to 'dumpsegs.bin'")
