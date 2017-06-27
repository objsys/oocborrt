# C to object file compilation rules

$(OBJDIR)$(PS)context$(OBJ): $(RTXSRCDIR)$(PS)context.c $(RTXSRCDIR)$(PS)rtxCommon.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h \
 $(RTXSRCDIR)$(PS)rtxMemory.h $(RTXSRCDIR)$(PS)rtxCharStr.h
	$(CC) $(RTXCFLAGS) -c $(IPATHS) $(OBJOUT) $(RTXSRCDIR)$(PS)context.c


$(OBJDIR)$(PS)file$(OBJ): $(RTXSRCDIR)$(PS)file.c $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxContext.h \
 $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)rtxStack.h \
 $(RTXSRCDIR)$(PS)rtxErrCodes.h $(RTXSRCDIR)$(PS)rtxFile.h $(RTXSRCDIR)$(PS)rtxMemory.h
	$(CC) $(RTXCFLAGS) -c $(IPATHS) $(OBJOUT) $(RTXSRCDIR)$(PS)file.c


$(OBJDIR)$(PS)membuf$(OBJ): $(RTXSRCDIR)$(PS)membuf.c $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxContext.h \
 $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)rtxStack.h \
 $(RTXSRCDIR)$(PS)rtxErrCodes.h $(RTXSRCDIR)$(PS)rtxMemory.h $(RTXSRCDIR)$(PS)rtxMemBuf.h
	$(CC) $(RTXCFLAGS) -c $(IPATHS) $(OBJOUT) $(RTXSRCDIR)$(PS)membuf.c


$(OBJDIR)$(PS)memmgmt$(OBJ): $(RTXSRCDIR)$(PS)memmgmt.c $(RTXSRCDIR)$(PS)rtxCommon.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h \
 $(RTXSRCDIR)$(PS)rtxMemory.h
	$(CC) $(RTXCFLAGS) -c $(IPATHS) $(OBJOUT) $(RTXSRCDIR)$(PS)memmgmt.c


$(OBJDIR)$(PS)rtxBigFloat$(OBJ): $(RTXSRCDIR)$(PS)rtxBigFloat.c $(RTXSRCDIR)$(PS)rtxBigFloat.h \
 $(RTXSRCDIR)$(PS)rtxBigInt.h $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h \
 $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)rtxStack.h \
 $(RTXSRCDIR)$(PS)rtxCharStr.h $(RTXSRCDIR)$(PS)rtxCtype.h $(RTXSRCDIR)$(PS)rtxError.h \
 $(RTXSRCDIR)$(PS)rtxErrCodes.h $(RTXSRCDIR)$(PS)rtxMemory.h $(RTXSRCDIR)$(PS)rtxReal.h
	$(CC) $(RTXCFLAGS) -c $(IPATHS) $(OBJOUT) $(RTXSRCDIR)$(PS)rtxBigFloat.c


$(OBJDIR)$(PS)rtxBigInt$(OBJ): $(RTXSRCDIR)$(PS)rtxBigInt.c $(RTXSRCDIR)$(PS)rtxBigInt.hh \
 $(RTXSRCDIR)$(PS)rtxBigInt.h $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h \
 $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)rtxStack.h \
 $(RTXSRCDIR)$(PS)rtxErrCodes.h $(RTXSRCDIR)$(PS)rtxCharStr.h $(RTXSRCDIR)$(PS)rtxCtype.h \
 $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxMemory.h $(RTXSRCDIR)$(PS)rtxReal.h
	$(CC) $(RTXCFLAGS) -c $(IPATHS) $(OBJOUT) $(RTXSRCDIR)$(PS)rtxBigInt.c


$(OBJDIR)$(PS)rtxBigIntCommon$(OBJ): $(RTXSRCDIR)$(PS)rtxBigIntCommon.c $(RTXSRCDIR)$(PS)rtxBigInt.hh \
 $(RTXSRCDIR)$(PS)rtxBigInt.h $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h \
 $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)rtxStack.h \
 $(RTXSRCDIR)$(PS)rtxErrCodes.h $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxMemory.h
	$(CC) $(RTXCFLAGS) -c $(IPATHS) $(OBJOUT) $(RTXSRCDIR)$(PS)rtxBigIntCommon.c


$(OBJDIR)$(PS)rtxCharStr$(OBJ): $(RTXSRCDIR)$(PS)rtxCharStr.c $(RTXSRCDIR)$(PS)rtxCharStr.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxCtype.h $(RTXSRCDIR)$(PS)rtxMemory.h
	$(CC) $(RTXCFLAGS) -c $(IPATHS) $(OBJOUT) $(RTXSRCDIR)$(PS)rtxCharStr.c


$(OBJDIR)$(PS)rtxDList$(OBJ): $(RTXSRCDIR)$(PS)rtxDList.c $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)rtxStack.h \
 $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h $(RTXSRCDIR)$(PS)rtxMemory.h \
 $(RTXSRCDIR)$(PS)rtxUTF8.h
	$(CC) $(RTXCFLAGS) -c $(IPATHS) $(OBJOUT) $(RTXSRCDIR)$(PS)rtxDList.c


$(OBJDIR)$(PS)rtxDateTime$(OBJ): $(RTXSRCDIR)$(PS)rtxDateTime.c $(RTXSRCDIR)$(PS)rtxDateTime.hh \
 $(RTXSRCDIR)$(PS)rtxDateTime.h $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h \
 $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)rtxStack.h \
 $(RTXSRCDIR)$(PS)rtxErrCodes.h $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxCtype.h
	$(CC) $(RTXCFLAGS) -c $(IPATHS) $(OBJOUT) $(RTXSRCDIR)$(PS)rtxDateTime.c


$(OBJDIR)$(PS)rtxDateTimeCompare$(OBJ): $(RTXSRCDIR)$(PS)rtxDateTimeCompare.c \
 $(RTXSRCDIR)$(PS)rtxDateTime.hh $(RTXSRCDIR)$(PS)rtxDateTime.h $(RTXSRCDIR)$(PS)rtxContext.h \
 $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)rtxStack.h \
 $(RTXSRCDIR)$(PS)rtxErrCodes.h $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxCtype.h
	$(CC) $(RTXCFLAGS) -c $(IPATHS) $(OBJOUT) $(RTXSRCDIR)$(PS)rtxDateTimeCompare.c


$(OBJDIR)$(PS)rtxDateTimeParse$(OBJ): $(RTXSRCDIR)$(PS)rtxDateTimeParse.c $(RTXSRCDIR)$(PS)rtxDateTime.hh \
 $(RTXSRCDIR)$(PS)rtxDateTime.h $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h \
 $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)rtxStack.h \
 $(RTXSRCDIR)$(PS)rtxErrCodes.h $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxCtype.h
	$(CC) $(RTXCFLAGS) -c $(IPATHS) $(OBJOUT) $(RTXSRCDIR)$(PS)rtxDateTimeParse.c


$(OBJDIR)$(PS)rtxDateTimeParseCommon$(OBJ): $(RTXSRCDIR)$(PS)rtxDateTimeParseCommon.c \
 $(RTXSRCDIR)$(PS)rtxDateTime.hh $(RTXSRCDIR)$(PS)rtxDateTime.h $(RTXSRCDIR)$(PS)rtxContext.h \
 $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)rtxStack.h \
 $(RTXSRCDIR)$(PS)rtxErrCodes.h $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxCtype.h
	$(CC) $(RTXCFLAGS) -c $(IPATHS) $(OBJOUT) $(RTXSRCDIR)$(PS)rtxDateTimeParseCommon.c


$(OBJDIR)$(PS)rtxDateTimeToString$(OBJ): $(RTXSRCDIR)$(PS)rtxDateTimeToString.c \
 $(RTXSRCDIR)$(PS)rtxCharStr.h $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h \
 $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)rtxStack.h \
 $(RTXSRCDIR)$(PS)rtxDateTime.hh $(RTXSRCDIR)$(PS)rtxDateTime.h $(RTXSRCDIR)$(PS)rtxErrCodes.h \
 $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxCtype.h $(RTXSRCDIR)$(PS)rtxUTF8.h
	$(CC) $(RTXCFLAGS) -c $(IPATHS) $(OBJOUT) $(RTXSRCDIR)$(PS)rtxDateTimeToString.c


$(OBJDIR)$(PS)rtxDateTimeToStringCommon$(OBJ): $(RTXSRCDIR)$(PS)rtxDateTimeToStringCommon.c \
 $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)rtxCharStr.h $(RTXSRCDIR)$(PS)rtxContext.h \
 $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxDateTime.hh \
 $(RTXSRCDIR)$(PS)rtxDateTime.h $(RTXSRCDIR)$(PS)rtxErrCodes.h $(RTXSRCDIR)$(PS)rtxError.h \
 $(RTXSRCDIR)$(PS)rtxCtype.h
	$(CC) $(RTXCFLAGS) -c $(IPATHS) $(OBJOUT) $(RTXSRCDIR)$(PS)rtxDateTimeToStringCommon.c


$(OBJDIR)$(PS)rtxError$(OBJ): $(RTXSRCDIR)$(PS)rtxError.c $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxBuffer.h $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h \
 $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxSList.h \
 $(RTXSRCDIR)$(PS)rtxCharStr.h $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h \
 $(RTXSRCDIR)$(PS)rtxMemory.h $(RTXSRCDIR)$(PS)rtxUTF8.h
	$(CC) $(RTXCFLAGS) -c $(IPATHS) $(OBJOUT) $(RTXSRCDIR)$(PS)rtxError.c


$(OBJDIR)$(PS)rtxGenValueType$(OBJ): $(RTXSRCDIR)$(PS)rtxGenValueType.c $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h \
 $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxErrCodes.h \
 $(RTXSRCDIR)$(PS)rtxGenValueType.h $(RTXSRCDIR)$(PS)rtxMemory.h $(RTXSRCDIR)$(PS)rtxHexDump.h
	$(CC) $(RTXCFLAGS) -c $(IPATHS) $(OBJOUT) $(RTXSRCDIR)$(PS)rtxGenValueType.c


$(OBJDIR)$(PS)rtxHexDump$(OBJ): $(RTXSRCDIR)$(PS)rtxHexDump.c $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxHexDump.h $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h \
 $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxCharStr.h \
 $(RTXSRCDIR)$(PS)rtxCtype.h $(RTXSRCDIR)$(PS)rtxErrCodes.h $(RTXSRCDIR)$(PS)rtxMemory.h
	$(CC) $(RTXCFLAGS) -c $(IPATHS) $(OBJOUT) $(RTXSRCDIR)$(PS)rtxHexDump.c


$(OBJDIR)$(PS)rtxHexDumpFile$(OBJ): $(RTXSRCDIR)$(PS)rtxHexDumpFile.c $(RTXSRCDIR)$(PS)rtxCharStr.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxCtype.h $(RTXSRCDIR)$(PS)rtxErrCodes.h \
 $(RTXSRCDIR)$(PS)rtxFile.h $(RTXSRCDIR)$(PS)rtxHexDump.h
	$(CC) $(RTXCFLAGS) -c $(IPATHS) $(OBJOUT) $(RTXSRCDIR)$(PS)rtxHexDumpFile.c


$(OBJDIR)$(PS)rtxIntToCharStr$(OBJ): $(RTXSRCDIR)$(PS)rtxIntToCharStr.c $(RTXSRCDIR)$(PS)rtxCharStr.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxErrCodes.h
	$(CC) $(RTXCFLAGS) -c $(IPATHS) $(OBJOUT) $(RTXSRCDIR)$(PS)rtxIntToCharStr.c


$(OBJDIR)$(PS)rtxInt64ToCharStr$(OBJ): $(RTXSRCDIR)$(PS)rtxInt64ToCharStr.c \
 $(RTXSRCDIR)$(PS)rtxIntToCharStr.c $(RTXSRCDIR)$(PS)rtxCharStr.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxErrCodes.h
	$(CC) $(RTXCFLAGS) -c $(IPATHS) $(OBJOUT) $(RTXSRCDIR)$(PS)rtxInt64ToCharStr.c


$(OBJDIR)$(PS)rtxIntStack$(OBJ): $(RTXSRCDIR)$(PS)rtxIntStack.c $(RTXSRCDIR)$(PS)rtxIntStack.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxMemory.h $(RTXSRCDIR)$(PS)rtxErrCodes.h
	$(CC) $(RTXCFLAGS) -c $(IPATHS) $(OBJOUT) $(RTXSRCDIR)$(PS)rtxIntStack.c


$(OBJDIR)$(PS)rtxPrint$(OBJ): $(RTXSRCDIR)$(PS)rtxPrint.c $(RTXSRCDIR)$(PS)rtxCtype.h \
 $(RTXSRCDIR)$(PS)rtxErrCodes.h $(RTXSRCDIR)$(PS)rtxPrint.h $(RTXSRCDIR)$(PS)rtxContext.h \
 $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)rtxStack.h \
 $(RTXSRCDIR)$(PS)rtxHexDump.h
	$(CC) $(RTXCFLAGS) -c $(IPATHS) $(OBJOUT) $(RTXSRCDIR)$(PS)rtxPrint.c


$(OBJDIR)$(PS)rtxRealHelper$(OBJ): $(RTXSRCDIR)$(PS)rtxRealHelper.c $(RTXSRCDIR)$(PS)rtxCommonDefs.h \
 $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)rtxReal.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h
	$(CC) $(RTXCFLAGS) -c $(IPATHS) $(OBJOUT) $(RTXSRCDIR)$(PS)rtxRealHelper.c


$(OBJDIR)$(PS)rtxSizeToCharStr$(OBJ): $(RTXSRCDIR)$(PS)rtxSizeToCharStr.c $(RTXSRCDIR)$(PS)rtxCharStr.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxErrCodes.h $(RTXSRCDIR)$(PS)rtxIntToCharStr.c
	$(CC) $(RTXCFLAGS) -c $(IPATHS) $(OBJOUT) $(RTXSRCDIR)$(PS)rtxSizeToCharStr.c


$(OBJDIR)$(PS)rtxSList$(OBJ): $(RTXSRCDIR)$(PS)rtxSList.c $(RTXSRCDIR)$(PS)rtxMemory.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxSList.h
	$(CC) $(RTXCFLAGS) -c $(IPATHS) $(OBJOUT) $(RTXSRCDIR)$(PS)rtxSList.c


$(OBJDIR)$(PS)rtxStack$(OBJ): $(RTXSRCDIR)$(PS)rtxStack.c $(RTXSRCDIR)$(PS)rtxMemory.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxErrCodes.h
	$(CC) $(RTXCFLAGS) -c $(IPATHS) $(OBJOUT) $(RTXSRCDIR)$(PS)rtxStack.c


$(OBJDIR)$(PS)rtxUIntToCharStr$(OBJ): $(RTXSRCDIR)$(PS)rtxUIntToCharStr.c $(RTXSRCDIR)$(PS)rtxCharStr.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxErrCodes.h $(RTXSRCDIR)$(PS)rtxIntToCharStr.c
	$(CC) $(RTXCFLAGS) -c $(IPATHS) $(OBJOUT) $(RTXSRCDIR)$(PS)rtxUIntToCharStr.c


$(OBJDIR)$(PS)rtxUInt64ToCharStr$(OBJ): $(RTXSRCDIR)$(PS)rtxUInt64ToCharStr.c \
 $(RTXSRCDIR)$(PS)rtxCharStr.h $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h \
 $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)rtxStack.h \
 $(RTXSRCDIR)$(PS)rtxErrCodes.h $(RTXSRCDIR)$(PS)rtxIntToCharStr.c
	$(CC) $(RTXCFLAGS) -c $(IPATHS) $(OBJOUT) $(RTXSRCDIR)$(PS)rtxUInt64ToCharStr.c


$(OBJDIR)$(PS)rtxUTF8$(OBJ): $(RTXSRCDIR)$(PS)rtxUTF8.c $(RTXSRCDIR)$(PS)rtxCtype.h $(RTXSRCDIR)$(PS)rtxError.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxErrCodes.h $(RTXSRCDIR)$(PS)rtxMemory.h \
 $(RTXSRCDIR)$(PS)rtxUTF8.h
	$(CC) $(RTXCFLAGS) -c $(IPATHS) $(OBJOUT) $(RTXSRCDIR)$(PS)rtxUTF8.c

