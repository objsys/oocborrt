# C to object file compilation rules

$(OBJDIR)$(PS)rtCborDecArray$(OBJ): $(CBORSRCDIR)$(PS)rtCborDecArray.c $(CBORSRCDIR)$(PS)osrtcbor.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxGenValueType.h $(RTXSRCDIR)$(PS)rtxBuffer.h \
 $(RTXSRCDIR)$(PS)rtxSList.h $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h \
 $(RTXSRCDIR)$(PS)rtxMemory.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(CBORSRCDIR)$(PS)rtCborDecArray.c


$(OBJDIR)$(PS)rtCborDecBigFloat$(OBJ): $(CBORSRCDIR)$(PS)rtCborDecBigFloat.c \
 $(CBORSRCDIR)$(PS)osrtcbor.h $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h \
 $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)rtxStack.h \
 $(RTXSRCDIR)$(PS)rtxGenValueType.h $(RTXSRCDIR)$(PS)rtxBigFloat.h \
 $(RTXSRCDIR)$(PS)rtxBigInt.h $(RTXSRCDIR)$(PS)rtxBuffer.h $(RTXSRCDIR)$(PS)rtxSList.h \
 $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(CBORSRCDIR)$(PS)rtCborDecBigFloat.c


$(OBJDIR)$(PS)rtCborDecBigInt$(OBJ): $(CBORSRCDIR)$(PS)rtCborDecBigInt.c $(CBORSRCDIR)$(PS)osrtcbor.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxGenValueType.h $(RTXSRCDIR)$(PS)rtxBigInt.h \
 $(RTXSRCDIR)$(PS)rtxBuffer.h $(RTXSRCDIR)$(PS)rtxSList.h $(RTXSRCDIR)$(PS)rtxDateTime.h \
 $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h $(RTXSRCDIR)$(PS)rtxMemory.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(CBORSRCDIR)$(PS)rtCborDecBigInt.c


$(OBJDIR)$(PS)rtCborDecByteStr$(OBJ): $(CBORSRCDIR)$(PS)rtCborDecByteStr.c \
 $(CBORSRCDIR)$(PS)osrtcbor.h $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h \
 $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)rtxStack.h \
 $(RTXSRCDIR)$(PS)rtxGenValueType.h $(RTXSRCDIR)$(PS)rtxBuffer.h $(RTXSRCDIR)$(PS)rtxSList.h \
 $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(CBORSRCDIR)$(PS)rtCborDecByteStr.c


$(OBJDIR)$(PS)rtCborDecDynByteStr$(OBJ): $(CBORSRCDIR)$(PS)rtCborDecDynByteStr.c \
 $(CBORSRCDIR)$(PS)osrtcbor.h $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h \
 $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)rtxStack.h \
 $(RTXSRCDIR)$(PS)rtxGenValueType.h $(RTXSRCDIR)$(PS)rtxBuffer.h $(RTXSRCDIR)$(PS)rtxSList.h \
 $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h $(RTXSRCDIR)$(PS)rtxMemory.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(CBORSRCDIR)$(PS)rtCborDecDynByteStr.c


$(OBJDIR)$(PS)rtCborDecDateTimeStr$(OBJ): $(CBORSRCDIR)$(PS)rtCborDecDateTimeStr.c \
 $(CBORSRCDIR)$(PS)osrtcbor.h $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h \
 $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)rtxStack.h \
 $(RTXSRCDIR)$(PS)rtxGenValueType.h $(RTXSRCDIR)$(PS)rtxBuffer.h $(RTXSRCDIR)$(PS)rtxSList.h \
 $(RTXSRCDIR)$(PS)rtxDateTime.h $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(CBORSRCDIR)$(PS)rtCborDecDateTimeStr.c


$(OBJDIR)$(PS)rtCborDecFloat$(OBJ): $(CBORSRCDIR)$(PS)rtCborDecFloat.c $(CBORSRCDIR)$(PS)osrtcbor.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxGenValueType.h $(RTXSRCDIR)$(PS)rtxBuffer.h \
 $(RTXSRCDIR)$(PS)rtxSList.h $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h \
 $(RTXSRCDIR)$(PS)rtxReal.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(CBORSRCDIR)$(PS)rtCborDecFloat.c


$(OBJDIR)$(PS)rtCborDecInt16$(OBJ): $(CBORSRCDIR)$(PS)rtCborDecInt16.c \
 $(CBORSRCDIR)$(PS)rtCborDecInt32.c $(CBORSRCDIR)$(PS)osrtcbor.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxGenValueType.h $(RTXSRCDIR)$(PS)rtxError.h \
 $(RTXSRCDIR)$(PS)rtxErrCodes.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(CBORSRCDIR)$(PS)rtCborDecInt16.c


$(OBJDIR)$(PS)rtCborDecInt32$(OBJ): $(CBORSRCDIR)$(PS)rtCborDecInt32.c $(CBORSRCDIR)$(PS)osrtcbor.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxGenValueType.h $(RTXSRCDIR)$(PS)rtxError.h \
 $(RTXSRCDIR)$(PS)rtxErrCodes.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(CBORSRCDIR)$(PS)rtCborDecInt32.c


$(OBJDIR)$(PS)rtCborDecInt64$(OBJ): $(CBORSRCDIR)$(PS)rtCborDecInt64.c \
 $(CBORSRCDIR)$(PS)rtCborDecInt32.c $(CBORSRCDIR)$(PS)osrtcbor.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxGenValueType.h $(RTXSRCDIR)$(PS)rtxError.h \
 $(RTXSRCDIR)$(PS)rtxErrCodes.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(CBORSRCDIR)$(PS)rtCborDecInt64.c


$(OBJDIR)$(PS)rtCborDecMap$(OBJ): $(CBORSRCDIR)$(PS)rtCborDecMap.c $(CBORSRCDIR)$(PS)osrtcbor.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxGenValueType.h $(RTXSRCDIR)$(PS)rtxError.h \
 $(RTXSRCDIR)$(PS)rtxErrCodes.h $(RTXSRCDIR)$(PS)rtxMemory.h $(RTXSRCDIR)$(PS)rtxSList.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(CBORSRCDIR)$(PS)rtCborDecMap.c


$(OBJDIR)$(PS)rtCborDecSize$(OBJ): $(CBORSRCDIR)$(PS)rtCborDecSize.c \
 $(CBORSRCDIR)$(PS)rtCborDecUInt32.c $(CBORSRCDIR)$(PS)osrtcbor.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxGenValueType.h $(RTXSRCDIR)$(PS)rtxBuffer.h \
 $(RTXSRCDIR)$(PS)rtxSList.h $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(CBORSRCDIR)$(PS)rtCborDecSize.c


$(OBJDIR)$(PS)rtCborDecSkipField$(OBJ): $(CBORSRCDIR)$(PS)rtCborDecSkipField.c \
 $(CBORSRCDIR)$(PS)osrtcbor.h $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h \
 $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)rtxStack.h \
 $(RTXSRCDIR)$(PS)rtxGenValueType.h $(RTXSRCDIR)$(PS)rtxBuffer.h $(RTXSRCDIR)$(PS)rtxSList.h \
 $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(CBORSRCDIR)$(PS)rtCborDecSkipField.c


$(OBJDIR)$(PS)rtCborDecTime$(OBJ): $(CBORSRCDIR)$(PS)rtCborDecTime.c $(CBORSRCDIR)$(PS)osrtcbor.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxGenValueType.h $(RTXSRCDIR)$(PS)rtxBuffer.h \
 $(RTXSRCDIR)$(PS)rtxSList.h $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(CBORSRCDIR)$(PS)rtCborDecTime.c


$(OBJDIR)$(PS)rtCborDecTimeValue$(OBJ): $(CBORSRCDIR)$(PS)rtCborDecTimeValue.c \
 $(CBORSRCDIR)$(PS)rtCborDecUInt32.c $(CBORSRCDIR)$(PS)osrtcbor.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxGenValueType.h $(RTXSRCDIR)$(PS)rtxBuffer.h \
 $(RTXSRCDIR)$(PS)rtxSList.h $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(CBORSRCDIR)$(PS)rtCborDecTimeValue.c


$(OBJDIR)$(PS)rtCborDecUInt16$(OBJ): $(CBORSRCDIR)$(PS)rtCborDecUInt16.c \
 $(CBORSRCDIR)$(PS)rtCborDecUInt32.c $(CBORSRCDIR)$(PS)osrtcbor.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxGenValueType.h $(RTXSRCDIR)$(PS)rtxBuffer.h \
 $(RTXSRCDIR)$(PS)rtxSList.h $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(CBORSRCDIR)$(PS)rtCborDecUInt16.c


$(OBJDIR)$(PS)rtCborDecUInt32$(OBJ): $(CBORSRCDIR)$(PS)rtCborDecUInt32.c $(CBORSRCDIR)$(PS)osrtcbor.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxGenValueType.h $(RTXSRCDIR)$(PS)rtxBuffer.h \
 $(RTXSRCDIR)$(PS)rtxSList.h $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(CBORSRCDIR)$(PS)rtCborDecUInt32.c


$(OBJDIR)$(PS)rtCborDecUInt64$(OBJ): $(CBORSRCDIR)$(PS)rtCborDecUInt64.c \
 $(CBORSRCDIR)$(PS)rtCborDecUInt32.c $(CBORSRCDIR)$(PS)osrtcbor.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxGenValueType.h $(RTXSRCDIR)$(PS)rtxBuffer.h \
 $(RTXSRCDIR)$(PS)rtxSList.h $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(CBORSRCDIR)$(PS)rtCborDecUInt64.c


$(OBJDIR)$(PS)rtCborDecUTF8Str$(OBJ): $(CBORSRCDIR)$(PS)rtCborDecUTF8Str.c \
 $(CBORSRCDIR)$(PS)osrtcbor.h $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h \
 $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)rtxStack.h \
 $(RTXSRCDIR)$(PS)rtxGenValueType.h $(RTXSRCDIR)$(PS)rtxBuffer.h $(RTXSRCDIR)$(PS)rtxSList.h \
 $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(CBORSRCDIR)$(PS)rtCborDecUTF8Str.c


$(OBJDIR)$(PS)rtCborDecDynUTF8Str$(OBJ): $(CBORSRCDIR)$(PS)rtCborDecDynUTF8Str.c \
 $(CBORSRCDIR)$(PS)osrtcbor.h $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h \
 $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)rtxStack.h \
 $(RTXSRCDIR)$(PS)rtxGenValueType.h $(RTXSRCDIR)$(PS)rtxBuffer.h $(RTXSRCDIR)$(PS)rtxSList.h \
 $(RTXSRCDIR)$(PS)rtxCharStr.h $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h \
 $(RTXSRCDIR)$(PS)rtxMemory.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(CBORSRCDIR)$(PS)rtCborDecDynUTF8Str.c


$(OBJDIR)$(PS)rtCborDecValue$(OBJ): $(CBORSRCDIR)$(PS)rtCborDecValue.c $(CBORSRCDIR)$(PS)osrtcbor.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxGenValueType.h $(RTXSRCDIR)$(PS)rtxBuffer.h \
 $(RTXSRCDIR)$(PS)rtxSList.h $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h \
 $(RTXSRCDIR)$(PS)rtxMemory.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(CBORSRCDIR)$(PS)rtCborDecValue.c


$(OBJDIR)$(PS)rtCborEncArray$(OBJ): $(CBORSRCDIR)$(PS)rtCborEncArray.c $(CBORSRCDIR)$(PS)osrtcbor.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxGenValueType.h $(RTXSRCDIR)$(PS)rtxBuffer.h \
 $(RTXSRCDIR)$(PS)rtxSList.h $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(CBORSRCDIR)$(PS)rtCborEncArray.c


$(OBJDIR)$(PS)rtCborEncBigFloat$(OBJ): $(CBORSRCDIR)$(PS)rtCborEncBigFloat.c \
 $(CBORSRCDIR)$(PS)osrtcbor.h $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h \
 $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)rtxStack.h \
 $(RTXSRCDIR)$(PS)rtxGenValueType.h $(RTXSRCDIR)$(PS)rtxBigFloat.h \
 $(RTXSRCDIR)$(PS)rtxBigInt.h $(RTXSRCDIR)$(PS)rtxBuffer.h $(RTXSRCDIR)$(PS)rtxSList.h \
 $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(CBORSRCDIR)$(PS)rtCborEncBigFloat.c


$(OBJDIR)$(PS)rtCborEncBigInt$(OBJ): $(CBORSRCDIR)$(PS)rtCborEncBigInt.c $(CBORSRCDIR)$(PS)osrtcbor.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxGenValueType.h $(RTXSRCDIR)$(PS)rtxBigInt.h \
 $(RTXSRCDIR)$(PS)rtxBuffer.h $(RTXSRCDIR)$(PS)rtxSList.h $(RTXSRCDIR)$(PS)rtxError.h \
 $(RTXSRCDIR)$(PS)rtxErrCodes.h $(RTXSRCDIR)$(PS)rtxMemory.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(CBORSRCDIR)$(PS)rtCborEncBigInt.c


$(OBJDIR)$(PS)rtCborEncByteStr$(OBJ): $(CBORSRCDIR)$(PS)rtCborEncByteStr.c \
 $(CBORSRCDIR)$(PS)osrtcbor.h $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h \
 $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)rtxStack.h \
 $(RTXSRCDIR)$(PS)rtxGenValueType.h $(RTXSRCDIR)$(PS)rtxBuffer.h $(RTXSRCDIR)$(PS)rtxSList.h \
 $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(CBORSRCDIR)$(PS)rtCborEncByteStr.c


$(OBJDIR)$(PS)rtCborEncDateTimeStr$(OBJ): $(CBORSRCDIR)$(PS)rtCborEncDateTimeStr.c \
 $(CBORSRCDIR)$(PS)osrtcbor.h $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h \
 $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)rtxStack.h \
 $(RTXSRCDIR)$(PS)rtxGenValueType.h $(RTXSRCDIR)$(PS)rtxBuffer.h $(RTXSRCDIR)$(PS)rtxSList.h \
 $(RTXSRCDIR)$(PS)rtxDateTime.h $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(CBORSRCDIR)$(PS)rtCborEncDateTimeStr.c


$(OBJDIR)$(PS)rtCborEncDecSimple$(OBJ): $(CBORSRCDIR)$(PS)rtCborEncDecSimple.c \
 $(CBORSRCDIR)$(PS)osrtcbor.h $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h \
 $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)rtxStack.h \
 $(RTXSRCDIR)$(PS)rtxGenValueType.h $(RTXSRCDIR)$(PS)rtxBuffer.h $(RTXSRCDIR)$(PS)rtxSList.h \
 $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h $(RTXSRCDIR)$(PS)rtxHexDump.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(CBORSRCDIR)$(PS)rtCborEncDecSimple.c


$(OBJDIR)$(PS)rtCborEncFloat$(OBJ): $(CBORSRCDIR)$(PS)rtCborEncFloat.c $(CBORSRCDIR)$(PS)osrtcbor.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxGenValueType.h $(RTXSRCDIR)$(PS)rtxBuffer.h \
 $(RTXSRCDIR)$(PS)rtxSList.h $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(CBORSRCDIR)$(PS)rtCborEncFloat.c


$(OBJDIR)$(PS)rtCborEncInt32$(OBJ): $(CBORSRCDIR)$(PS)rtCborEncInt32.c $(CBORSRCDIR)$(PS)osrtcbor.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxGenValueType.h $(RTXSRCDIR)$(PS)rtxBuffer.h \
 $(RTXSRCDIR)$(PS)rtxSList.h $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(CBORSRCDIR)$(PS)rtCborEncInt32.c


$(OBJDIR)$(PS)rtCborEncInt64$(OBJ): $(CBORSRCDIR)$(PS)rtCborEncInt64.c \
 $(CBORSRCDIR)$(PS)rtCborEncInt32.c $(CBORSRCDIR)$(PS)osrtcbor.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxGenValueType.h $(RTXSRCDIR)$(PS)rtxBuffer.h \
 $(RTXSRCDIR)$(PS)rtxSList.h $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(CBORSRCDIR)$(PS)rtCborEncInt64.c


$(OBJDIR)$(PS)rtCborEncMap$(OBJ): $(CBORSRCDIR)$(PS)rtCborEncMap.c $(CBORSRCDIR)$(PS)osrtcbor.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxGenValueType.h $(RTXSRCDIR)$(PS)rtxBuffer.h \
 $(RTXSRCDIR)$(PS)rtxSList.h $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(CBORSRCDIR)$(PS)rtCborEncMap.c


$(OBJDIR)$(PS)rtCborEncNumberStr$(OBJ): $(CBORSRCDIR)$(PS)rtCborEncNumberStr.c \
 $(CBORSRCDIR)$(PS)osrtcbor.h $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h \
 $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)rtxStack.h \
 $(RTXSRCDIR)$(PS)rtxGenValueType.h $(RTXSRCDIR)$(PS)rtxBigFloat.h \
 $(RTXSRCDIR)$(PS)rtxBigInt.h $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(CBORSRCDIR)$(PS)rtCborEncNumberStr.c


$(OBJDIR)$(PS)rtCborEncSize$(OBJ): $(CBORSRCDIR)$(PS)rtCborEncSize.c \
 $(CBORSRCDIR)$(PS)rtCborEncUInt32.c $(CBORSRCDIR)$(PS)osrtcbor.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxGenValueType.h $(RTXSRCDIR)$(PS)rtxBuffer.h \
 $(RTXSRCDIR)$(PS)rtxSList.h $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(CBORSRCDIR)$(PS)rtCborEncSize.c


$(OBJDIR)$(PS)rtCborEncTime$(OBJ): $(CBORSRCDIR)$(PS)rtCborEncTime.c $(CBORSRCDIR)$(PS)osrtcbor.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxGenValueType.h $(RTXSRCDIR)$(PS)rtxBuffer.h \
 $(RTXSRCDIR)$(PS)rtxSList.h $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(CBORSRCDIR)$(PS)rtCborEncTime.c


$(OBJDIR)$(PS)rtCborEncTimeValue$(OBJ): $(CBORSRCDIR)$(PS)rtCborEncTimeValue.c \
 $(CBORSRCDIR)$(PS)rtCborEncUInt32.c $(CBORSRCDIR)$(PS)osrtcbor.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxGenValueType.h $(RTXSRCDIR)$(PS)rtxBuffer.h \
 $(RTXSRCDIR)$(PS)rtxSList.h $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(CBORSRCDIR)$(PS)rtCborEncTimeValue.c


$(OBJDIR)$(PS)rtCborEncUInt32$(OBJ): $(CBORSRCDIR)$(PS)rtCborEncUInt32.c $(CBORSRCDIR)$(PS)osrtcbor.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxGenValueType.h $(RTXSRCDIR)$(PS)rtxBuffer.h \
 $(RTXSRCDIR)$(PS)rtxSList.h $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(CBORSRCDIR)$(PS)rtCborEncUInt32.c


$(OBJDIR)$(PS)rtCborEncUInt64$(OBJ): $(CBORSRCDIR)$(PS)rtCborEncUInt64.c \
 $(CBORSRCDIR)$(PS)rtCborEncUInt32.c $(CBORSRCDIR)$(PS)osrtcbor.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxGenValueType.h $(RTXSRCDIR)$(PS)rtxBuffer.h \
 $(RTXSRCDIR)$(PS)rtxSList.h $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(CBORSRCDIR)$(PS)rtCborEncUInt64.c


$(OBJDIR)$(PS)rtCborEncUTF8Str$(OBJ): $(CBORSRCDIR)$(PS)rtCborEncUTF8Str.c \
 $(CBORSRCDIR)$(PS)osrtcbor.h $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h \
 $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)rtxStack.h \
 $(RTXSRCDIR)$(PS)rtxGenValueType.h $(RTXSRCDIR)$(PS)rtxBuffer.h $(RTXSRCDIR)$(PS)rtxSList.h \
 $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(CBORSRCDIR)$(PS)rtCborEncUTF8Str.c


$(OBJDIR)$(PS)rtCborEncValue$(OBJ): $(CBORSRCDIR)$(PS)rtCborEncValue.c $(CBORSRCDIR)$(PS)osrtcbor.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxGenValueType.h $(RTXSRCDIR)$(PS)rtxBuffer.h \
 $(RTXSRCDIR)$(PS)rtxSList.h $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h \
 $(RTXSRCDIR)$(PS)rtxMemory.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(CBORSRCDIR)$(PS)rtCborEncValue.c

