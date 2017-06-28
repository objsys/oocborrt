# makefile to build generated files

include ../platform.mk

# include directories
OSROOTDIR = ..
LIBDIR  = $(OSROOTDIR)$(PS)lib

# compiler defs
CFLAGS = -c $(CVARS_) $(CFLAGS_) $(CBLDTYPE_)
IPATHS = -I$(OSROOTDIR) $(IPATHS_)
LINKOPT = $(LINKOPT_)

# run-time libraries
LIBS = $(LIBDIR)$(PS)$(LIBPFX)oortcbor$(A)

LLIBS = $(LLCBOR) $(LLSYS)
LPATHS = $(LPPFX)$(LIBDIR)

all : json2cbor$(EXE) cbor2json$(EXE)

json2cbor$(EXE) : json2cbor$(OBJ) $(LIBS)
	$(LINK) json2cbor$(OBJ) $(LINKOPT) $(LPATHS) $(LLIBS)

cbor2json$(EXE) : cbor2json$(OBJ) $(LIBS)
	$(LINK) cbor2json$(OBJ) $(LINKOPT) $(LPATHS) $(LLIBS)

# .c -> .obj rules
json2cbor$(OBJ) : json2cbor.c ../rtjsonsrc/osrtjson.h ../rtxsrc/osMacros.h \
 ../rtxsrc/osSysTypes.h ../rtxsrc/rtxCommon.h ../rtxsrc/rtxContext.h \
 ../rtxsrc/rtxDList.h ../rtxsrc/rtxExternDefs.h ../rtxsrc/rtxCommonDefs.h \
 ../rtxsrc/rtxStack.h ../rtxsrc/rtxError.h ../rtxsrc/rtxErrCodes.h \
 ../rtxsrc/rtxMemory.h ../rtxsrc/rtxBuffer.h ../rtxsrc/rtxSList.h \
 ../rtjsonsrc/rtJsonExternDefs.h ../rtcborsrc/osrtcbor.h \
 ../rtxsrc/rtxGenValueType.h ../rtxsrc/rtxFile.h ../rtxsrc/rtxHexDump.h
	$(CC) $(CFLAGS) -c $(IPATHS) json2cbor.c

cbor2json$(OBJ): cbor2json.c ../rtjsonsrc/osrtjson.h ../rtxsrc/osMacros.h \
 ../rtxsrc/osSysTypes.h ../rtxsrc/rtxCommon.h ../rtxsrc/rtxContext.h \
 ../rtxsrc/rtxDList.h ../rtxsrc/rtxExternDefs.h ../rtxsrc/rtxCommonDefs.h \
 ../rtxsrc/rtxStack.h ../rtxsrc/rtxError.h ../rtxsrc/rtxErrCodes.h \
 ../rtxsrc/rtxMemory.h ../rtxsrc/rtxBuffer.h ../rtxsrc/rtxSList.h \
 ../rtjsonsrc/rtJsonExternDefs.h ../rtcborsrc/osrtcbor.h \
 ../rtxsrc/rtxGenValueType.h ../rtxsrc/rtxFile.h ../rtxsrc/rtxHexDump.h
	$(CC) $(CFLAGS) -c $(IPATHS) cbor2json.c

depends:
	gcc -E -MM $(IPATHS) json2cbor.c
	gcc -E -MM $(IPATHS) cbor2json.c

clean:
	$(RM) *$(OBJ)
	$(RM) *.exe
	$(RM) *.exp
	$(RM) *.pdb
	$(RM) *.map
	$(RM) *.lib
	$(RM) *~
	$(RM) json2cbor$(EXE)
	$(RM) cbor2json$(EXE)
