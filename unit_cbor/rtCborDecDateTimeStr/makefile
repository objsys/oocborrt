include ../../platform.mk

CFLAGS	= -D_TRACE $(CBLDTYPE_) $(CVARSMDD_) $(MCFLAGS) $(CFLAGS_)
IPATHS = -I../../
LIBDIR = ../../lib
LPATHS = $(LPPFX)$(LIBDIR)

unittest$(EXE): unittest$(OBJ) $(LIBDIR)/$(LIBPFX)osrtcbor$(A)
	$(LINK) unittest$(OBJ) $(LINKOPT_) $(LPATHS) $(LLCBOR) $(LLSYS)

unittest$(OBJ): unittest.c ../../../rtcborsrc/osrtcbor.h \
 ../../../rtxsrc/rtxContext.h ../../../rtxsrc/rtxDList.h \
 ../../../rtxsrc/osSysTypes.h ../../../rtxsrc/rtxExternDefs.h \
 ../../../rtxsrc/rtxCommonDefs.h ../../../rtxsrc/osMacros.h \
 ../../../rtxsrc/rtxStack.h ../../../rtxsrc/rtxGenValueType.h \
 ../../../rtxsrc/rtxBuffer.h ../../../rtxsrc/rtxSList.h \
 ../../../rtxsrc/rtxDateTime.h ../../../rtxsrc/rtxError.h \
 ../../../rtxsrc/rtxErrCodes.h ../../../rtxsrc/rtxPrint.h

depends:
	gcc -E -MM $(IPATHS) unittest.c

clean:
	$(RM) *$(OBJ)
	$(RM) *.exe
	$(RM) *.exp
	$(RM) *.pdb
	$(RM) *.map
	$(RM) *.lib
	$(RM) *~
	$(RM) unittest$(EXE)
