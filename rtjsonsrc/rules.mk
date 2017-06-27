# C to object file compilation rules

$(OBJDIR)$(PS)rtJsonParser$(OBJ): $(JSONSRCDIR)$(PS)rtJsonParser.c $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxBuffer.h $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h \
 $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxSList.h \
 $(RTXSRCDIR)$(PS)rtxCtype.h $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h \
 $(RTXSRCDIR)$(PS)rtxUTF8.h $(JSONSRCDIR)$(PS)osrtjson.h $(RTXSRCDIR)$(PS)rtxCommon.h \
 $(RTXSRCDIR)$(PS)rtxMemory.h $(JSONSRCDIR)$(PS)rtJsonExternDefs.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(JSONSRCDIR)$(PS)rtJsonParser.c


$(OBJDIR)$(PS)rtJsonDecAny$(OBJ): $(JSONSRCDIR)$(PS)rtJsonDecAny.c $(RTXSRCDIR)$(PS)rtxBuffer.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxSList.h $(RTXSRCDIR)$(PS)rtxCtype.h \
 $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h $(RTXSRCDIR)$(PS)rtxIntStack.h \
 $(RTXSRCDIR)$(PS)rtxMemBuf.h $(JSONSRCDIR)$(PS)osrtjson.h $(RTXSRCDIR)$(PS)rtxCommon.h \
 $(RTXSRCDIR)$(PS)rtxMemory.h $(JSONSRCDIR)$(PS)rtJsonExternDefs.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(JSONSRCDIR)$(PS)rtJsonDecAny.c


$(OBJDIR)$(PS)rtJsonDecBase64Str$(OBJ): $(JSONSRCDIR)$(PS)rtJsonDecBase64Str.c \
 $(JSONSRCDIR)$(PS)osrtjson.h $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxCommon.h $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)rtxStack.h \
 $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h $(RTXSRCDIR)$(PS)rtxMemory.h \
 $(RTXSRCDIR)$(PS)rtxBuffer.h $(RTXSRCDIR)$(PS)rtxSList.h \
 $(JSONSRCDIR)$(PS)rtJsonExternDefs.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(JSONSRCDIR)$(PS)rtJsonDecBase64Str.c


$(OBJDIR)$(PS)rtJsonDecDynBase64Str$(OBJ): $(JSONSRCDIR)$(PS)rtJsonDecDynBase64Str.c \
 $(RTXSRCDIR)$(PS)rtxMemBuf.h $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h \
 $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)rtxStack.h \
 $(RTXSRCDIR)$(PS)rtxCtype.h $(JSONSRCDIR)$(PS)osrtjson.h $(RTXSRCDIR)$(PS)rtxCommon.h \
 $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h $(RTXSRCDIR)$(PS)rtxMemory.h \
 $(RTXSRCDIR)$(PS)rtxBuffer.h $(RTXSRCDIR)$(PS)rtxSList.h \
 $(JSONSRCDIR)$(PS)rtJsonExternDefs.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(JSONSRCDIR)$(PS)rtJsonDecDynBase64Str.c


$(OBJDIR)$(PS)rtJsonDecBool$(OBJ): $(JSONSRCDIR)$(PS)rtJsonDecBool.c $(JSONSRCDIR)$(PS)osrtjson.h \
 $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxCommon.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxError.h \
 $(RTXSRCDIR)$(PS)rtxErrCodes.h $(RTXSRCDIR)$(PS)rtxMemory.h $(RTXSRCDIR)$(PS)rtxBuffer.h \
 $(RTXSRCDIR)$(PS)rtxSList.h $(JSONSRCDIR)$(PS)rtJsonExternDefs.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(JSONSRCDIR)$(PS)rtJsonDecBool.c


$(OBJDIR)$(PS)rtJsonDecDateTime$(OBJ): $(JSONSRCDIR)$(PS)rtJsonDecDateTime.c \
 $(RTXSRCDIR)$(PS)rtxCtype.h $(JSONSRCDIR)$(PS)osrtjson.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxCommon.h $(RTXSRCDIR)$(PS)rtxContext.h \
 $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h \
 $(RTXSRCDIR)$(PS)rtxMemory.h $(RTXSRCDIR)$(PS)rtxBuffer.h $(RTXSRCDIR)$(PS)rtxSList.h \
 $(JSONSRCDIR)$(PS)rtJsonExternDefs.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(JSONSRCDIR)$(PS)rtJsonDecDateTime.c


$(OBJDIR)$(PS)rtJsonDecDecimal$(OBJ): $(JSONSRCDIR)$(PS)rtJsonDecDecimal.c $(RTXSRCDIR)$(PS)rtxCtype.h \
 $(JSONSRCDIR)$(PS)osrtjson.h $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxCommon.h $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)rtxStack.h \
 $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h $(RTXSRCDIR)$(PS)rtxMemory.h \
 $(RTXSRCDIR)$(PS)rtxBuffer.h $(RTXSRCDIR)$(PS)rtxSList.h \
 $(JSONSRCDIR)$(PS)rtJsonExternDefs.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(JSONSRCDIR)$(PS)rtJsonDecDecimal.c


$(OBJDIR)$(PS)rtJsonDecDouble$(OBJ): $(JSONSRCDIR)$(PS)rtJsonDecDouble.c $(RTXSRCDIR)$(PS)rtxCtype.h \
 $(RTXSRCDIR)$(PS)rtxReal.h $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(JSONSRCDIR)$(PS)osrtjson.h $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)rtxCommon.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h \
 $(RTXSRCDIR)$(PS)rtxMemory.h $(RTXSRCDIR)$(PS)rtxBuffer.h $(RTXSRCDIR)$(PS)rtxSList.h \
 $(JSONSRCDIR)$(PS)rtJsonExternDefs.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(JSONSRCDIR)$(PS)rtJsonDecDouble.c


$(OBJDIR)$(PS)rtJsonDecDynHexStr$(OBJ): $(JSONSRCDIR)$(PS)rtJsonDecDynHexStr.c \
 $(RTXSRCDIR)$(PS)rtxMemBuf.h $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h \
 $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)rtxStack.h \
 $(JSONSRCDIR)$(PS)osrtjson.h $(RTXSRCDIR)$(PS)rtxCommon.h $(RTXSRCDIR)$(PS)rtxError.h \
 $(RTXSRCDIR)$(PS)rtxErrCodes.h $(RTXSRCDIR)$(PS)rtxMemory.h $(RTXSRCDIR)$(PS)rtxBuffer.h \
 $(RTXSRCDIR)$(PS)rtxSList.h $(JSONSRCDIR)$(PS)rtJsonExternDefs.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(JSONSRCDIR)$(PS)rtJsonDecDynHexStr.c


$(OBJDIR)$(PS)rtJsonDecHexStr$(OBJ): $(JSONSRCDIR)$(PS)rtJsonDecHexStr.c $(JSONSRCDIR)$(PS)osrtjson.h \
 $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxCommon.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxError.h \
 $(RTXSRCDIR)$(PS)rtxErrCodes.h $(RTXSRCDIR)$(PS)rtxMemory.h $(RTXSRCDIR)$(PS)rtxBuffer.h \
 $(RTXSRCDIR)$(PS)rtxSList.h $(JSONSRCDIR)$(PS)rtJsonExternDefs.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(JSONSRCDIR)$(PS)rtJsonDecHexStr.c


$(OBJDIR)$(PS)rtJsonDecInt64$(OBJ): $(JSONSRCDIR)$(PS)rtJsonDecInt64.c $(RTXSRCDIR)$(PS)rtxBuffer.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxSList.h $(RTXSRCDIR)$(PS)rtxCtype.h \
 $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h $(JSONSRCDIR)$(PS)osrtjson.h \
 $(RTXSRCDIR)$(PS)rtxCommon.h $(RTXSRCDIR)$(PS)rtxMemory.h \
 $(JSONSRCDIR)$(PS)rtJsonExternDefs.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(JSONSRCDIR)$(PS)rtJsonDecInt64.c


$(OBJDIR)$(PS)rtJsonDecInt32$(OBJ): $(JSONSRCDIR)$(PS)rtJsonDecInt32.c $(RTXSRCDIR)$(PS)rtxBuffer.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxSList.h $(RTXSRCDIR)$(PS)rtxCtype.h \
 $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h $(JSONSRCDIR)$(PS)osrtjson.h \
 $(RTXSRCDIR)$(PS)rtxCommon.h $(RTXSRCDIR)$(PS)rtxMemory.h \
 $(JSONSRCDIR)$(PS)rtJsonExternDefs.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(JSONSRCDIR)$(PS)rtJsonDecInt32.c


$(OBJDIR)$(PS)rtJsonDecInt16$(OBJ): $(JSONSRCDIR)$(PS)rtJsonDecInt16.c $(RTXSRCDIR)$(PS)rtxError.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxErrCodes.h $(JSONSRCDIR)$(PS)osrtjson.h \
 $(RTXSRCDIR)$(PS)rtxCommon.h $(RTXSRCDIR)$(PS)rtxMemory.h $(RTXSRCDIR)$(PS)rtxBuffer.h \
 $(RTXSRCDIR)$(PS)rtxSList.h $(JSONSRCDIR)$(PS)rtJsonExternDefs.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(JSONSRCDIR)$(PS)rtJsonDecInt16.c


$(OBJDIR)$(PS)rtJsonDecInt8$(OBJ): $(JSONSRCDIR)$(PS)rtJsonDecInt8.c $(RTXSRCDIR)$(PS)rtxError.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxErrCodes.h $(JSONSRCDIR)$(PS)osrtjson.h \
 $(RTXSRCDIR)$(PS)rtxCommon.h $(RTXSRCDIR)$(PS)rtxMemory.h $(RTXSRCDIR)$(PS)rtxBuffer.h \
 $(RTXSRCDIR)$(PS)rtxSList.h $(JSONSRCDIR)$(PS)rtJsonExternDefs.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(JSONSRCDIR)$(PS)rtJsonDecInt8.c


$(OBJDIR)$(PS)rtJsonDecNumberString$(OBJ): $(JSONSRCDIR)$(PS)rtJsonDecNumberString.c \
 $(JSONSRCDIR)$(PS)osrtjson.h $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxCommon.h $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)rtxStack.h \
 $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h $(RTXSRCDIR)$(PS)rtxMemory.h \
 $(RTXSRCDIR)$(PS)rtxBuffer.h $(RTXSRCDIR)$(PS)rtxSList.h \
 $(JSONSRCDIR)$(PS)rtJsonExternDefs.h $(RTXSRCDIR)$(PS)rtxCharStr.h \
 $(RTXSRCDIR)$(PS)rtxCtype.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(JSONSRCDIR)$(PS)rtJsonDecNumberString.c


$(OBJDIR)$(PS)rtJsonDecString$(OBJ): $(JSONSRCDIR)$(PS)rtJsonDecString.c $(RTXSRCDIR)$(PS)rtxBuffer.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxSList.h $(RTXSRCDIR)$(PS)rtxCtype.h \
 $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h $(RTXSRCDIR)$(PS)rtxMemBuf.h \
 $(JSONSRCDIR)$(PS)osrtjson.h $(RTXSRCDIR)$(PS)rtxCommon.h $(RTXSRCDIR)$(PS)rtxMemory.h \
 $(JSONSRCDIR)$(PS)rtJsonExternDefs.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(JSONSRCDIR)$(PS)rtJsonDecString.c


$(OBJDIR)$(PS)rtJsonDecUCS2String$(OBJ): $(JSONSRCDIR)$(PS)rtJsonDecUCS2String.c \
 $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)rtxErrCodes.h $(JSONSRCDIR)$(PS)osrtjson.h \
 $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxCommon.h $(RTXSRCDIR)$(PS)rtxContext.h \
 $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxMemory.h \
 $(RTXSRCDIR)$(PS)rtxBuffer.h $(RTXSRCDIR)$(PS)rtxSList.h \
 $(JSONSRCDIR)$(PS)rtJsonExternDefs.h $(JSONSRCDIR)$(PS)rtJsonDecUTF.c \
 $(RTXSRCDIR)$(PS)rtxMemBuf.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(JSONSRCDIR)$(PS)rtJsonDecUCS2String.c


$(OBJDIR)$(PS)rtJsonDecUCS4String$(OBJ): $(JSONSRCDIR)$(PS)rtJsonDecUCS4String.c \
 $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)rtxErrCodes.h $(JSONSRCDIR)$(PS)osrtjson.h \
 $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxCommon.h $(RTXSRCDIR)$(PS)rtxContext.h \
 $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxMemory.h \
 $(RTXSRCDIR)$(PS)rtxBuffer.h $(RTXSRCDIR)$(PS)rtxSList.h \
 $(JSONSRCDIR)$(PS)rtJsonExternDefs.h $(JSONSRCDIR)$(PS)rtJsonDecUTF.c \
 $(RTXSRCDIR)$(PS)rtxMemBuf.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(JSONSRCDIR)$(PS)rtJsonDecUCS4String.c


$(OBJDIR)$(PS)rtJsonDecUInt64$(OBJ): $(JSONSRCDIR)$(PS)rtJsonDecUInt64.c $(RTXSRCDIR)$(PS)rtxBuffer.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxSList.h $(RTXSRCDIR)$(PS)rtxCtype.h \
 $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h $(JSONSRCDIR)$(PS)osrtjson.h \
 $(RTXSRCDIR)$(PS)rtxCommon.h $(RTXSRCDIR)$(PS)rtxMemory.h \
 $(JSONSRCDIR)$(PS)rtJsonExternDefs.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(JSONSRCDIR)$(PS)rtJsonDecUInt64.c


$(OBJDIR)$(PS)rtJsonDecUInt32$(OBJ): $(JSONSRCDIR)$(PS)rtJsonDecUInt32.c $(RTXSRCDIR)$(PS)rtxBuffer.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxSList.h $(RTXSRCDIR)$(PS)rtxCtype.h \
 $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h $(JSONSRCDIR)$(PS)osrtjson.h \
 $(RTXSRCDIR)$(PS)rtxCommon.h $(RTXSRCDIR)$(PS)rtxMemory.h \
 $(JSONSRCDIR)$(PS)rtJsonExternDefs.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(JSONSRCDIR)$(PS)rtJsonDecUInt32.c


$(OBJDIR)$(PS)rtJsonDecUInt16$(OBJ): $(JSONSRCDIR)$(PS)rtJsonDecUInt16.c $(RTXSRCDIR)$(PS)rtxError.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxErrCodes.h $(JSONSRCDIR)$(PS)osrtjson.h \
 $(RTXSRCDIR)$(PS)rtxCommon.h $(RTXSRCDIR)$(PS)rtxMemory.h $(RTXSRCDIR)$(PS)rtxBuffer.h \
 $(RTXSRCDIR)$(PS)rtxSList.h $(JSONSRCDIR)$(PS)rtJsonExternDefs.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(JSONSRCDIR)$(PS)rtJsonDecUInt16.c


$(OBJDIR)$(PS)rtJsonDecUInt8$(OBJ): $(JSONSRCDIR)$(PS)rtJsonDecUInt8.c $(RTXSRCDIR)$(PS)rtxError.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxErrCodes.h $(JSONSRCDIR)$(PS)osrtjson.h \
 $(RTXSRCDIR)$(PS)rtxCommon.h $(RTXSRCDIR)$(PS)rtxMemory.h $(RTXSRCDIR)$(PS)rtxBuffer.h \
 $(RTXSRCDIR)$(PS)rtxSList.h $(JSONSRCDIR)$(PS)rtJsonExternDefs.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(JSONSRCDIR)$(PS)rtJsonDecUInt8.c


$(OBJDIR)$(PS)rtJsonEncAnyAttr$(OBJ): $(JSONSRCDIR)$(PS)rtJsonEncAnyAttr.c \
 $(JSONSRCDIR)$(PS)osrtjson.h $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxCommon.h $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)rtxStack.h \
 $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h $(RTXSRCDIR)$(PS)rtxMemory.h \
 $(RTXSRCDIR)$(PS)rtxBuffer.h $(RTXSRCDIR)$(PS)rtxSList.h \
 $(JSONSRCDIR)$(PS)rtJsonExternDefs.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(JSONSRCDIR)$(PS)rtJsonEncAnyAttr.c


$(OBJDIR)$(PS)rtJsonEncBase64Binary$(OBJ): $(JSONSRCDIR)$(PS)rtJsonEncBase64Binary.c \
 $(JSONSRCDIR)$(PS)osrtjson.h $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxCommon.h $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)rtxStack.h \
 $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h $(RTXSRCDIR)$(PS)rtxMemory.h \
 $(RTXSRCDIR)$(PS)rtxBuffer.h $(RTXSRCDIR)$(PS)rtxSList.h \
 $(JSONSRCDIR)$(PS)rtJsonExternDefs.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(JSONSRCDIR)$(PS)rtJsonEncBase64Binary.c


$(OBJDIR)$(PS)rtJsonEncBool$(OBJ): $(JSONSRCDIR)$(PS)rtJsonEncBool.c $(JSONSRCDIR)$(PS)osrtjson.h \
 $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxCommon.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxError.h \
 $(RTXSRCDIR)$(PS)rtxErrCodes.h $(RTXSRCDIR)$(PS)rtxMemory.h $(RTXSRCDIR)$(PS)rtxBuffer.h \
 $(RTXSRCDIR)$(PS)rtxSList.h $(JSONSRCDIR)$(PS)rtJsonExternDefs.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(JSONSRCDIR)$(PS)rtJsonEncBool.c


$(OBJDIR)$(PS)rtJsonEncDates$(OBJ): $(JSONSRCDIR)$(PS)rtJsonEncDates.c $(RTXSRCDIR)$(PS)rtxErrCodes.h \
 $(RTXSRCDIR)$(PS)rtxDateTime.hh $(RTXSRCDIR)$(PS)rtxDateTime.h $(RTXSRCDIR)$(PS)rtxContext.h \
 $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)rtxStack.h \
 $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxCtype.h $(RTXSRCDIR)$(PS)rtxBuffer.h \
 $(RTXSRCDIR)$(PS)rtxSList.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(JSONSRCDIR)$(PS)rtJsonEncDates.c


$(OBJDIR)$(PS)rtJsonEncDateTime$(OBJ): $(JSONSRCDIR)$(PS)rtJsonEncDateTime.c \
 $(JSONSRCDIR)$(PS)osrtjson.h $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxCommon.h $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)rtxStack.h \
 $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h $(RTXSRCDIR)$(PS)rtxMemory.h \
 $(RTXSRCDIR)$(PS)rtxBuffer.h $(RTXSRCDIR)$(PS)rtxSList.h \
 $(JSONSRCDIR)$(PS)rtJsonExternDefs.h $(RTXSRCDIR)$(PS)rtxDateTime.hh \
 $(RTXSRCDIR)$(PS)rtxDateTime.h $(RTXSRCDIR)$(PS)rtxCtype.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(JSONSRCDIR)$(PS)rtJsonEncDateTime.c


$(OBJDIR)$(PS)rtJsonEncDecimal$(OBJ): $(JSONSRCDIR)$(PS)rtJsonEncDecimal.c \
 $(JSONSRCDIR)$(PS)osrtjson.h $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxCommon.h $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)rtxStack.h \
 $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h $(RTXSRCDIR)$(PS)rtxMemory.h \
 $(RTXSRCDIR)$(PS)rtxBuffer.h $(RTXSRCDIR)$(PS)rtxSList.h \
 $(JSONSRCDIR)$(PS)rtJsonExternDefs.h $(RTXSRCDIR)$(PS)rtxCharStr.h \
 $(RTXSRCDIR)$(PS)rtxReal.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(JSONSRCDIR)$(PS)rtJsonEncDecimal.c


$(OBJDIR)$(PS)rtJsonEncDouble$(OBJ): $(JSONSRCDIR)$(PS)rtJsonEncDouble.c $(JSONSRCDIR)$(PS)osrtjson.h \
 $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxCommon.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxError.h \
 $(RTXSRCDIR)$(PS)rtxErrCodes.h $(RTXSRCDIR)$(PS)rtxMemory.h $(RTXSRCDIR)$(PS)rtxBuffer.h \
 $(RTXSRCDIR)$(PS)rtxSList.h $(JSONSRCDIR)$(PS)rtJsonExternDefs.h $(RTXSRCDIR)$(PS)rtxReal.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(JSONSRCDIR)$(PS)rtJsonEncDouble.c


$(OBJDIR)$(PS)rtJsonEncFloat$(OBJ): $(JSONSRCDIR)$(PS)rtJsonEncFloat.c \
 $(JSONSRCDIR)$(PS)rtJsonEncDouble.c $(JSONSRCDIR)$(PS)osrtjson.h \
 $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxCommon.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxError.h \
 $(RTXSRCDIR)$(PS)rtxErrCodes.h $(RTXSRCDIR)$(PS)rtxMemory.h $(RTXSRCDIR)$(PS)rtxBuffer.h \
 $(RTXSRCDIR)$(PS)rtxSList.h $(JSONSRCDIR)$(PS)rtJsonExternDefs.h $(RTXSRCDIR)$(PS)rtxReal.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(JSONSRCDIR)$(PS)rtJsonEncFloat.c


$(OBJDIR)$(PS)rtJsonEncHexBinary$(OBJ): $(JSONSRCDIR)$(PS)rtJsonEncHexBinary.c \
 $(JSONSRCDIR)$(PS)osrtjson.h $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxCommon.h $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)rtxStack.h \
 $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h $(RTXSRCDIR)$(PS)rtxMemory.h \
 $(RTXSRCDIR)$(PS)rtxBuffer.h $(RTXSRCDIR)$(PS)rtxSList.h \
 $(JSONSRCDIR)$(PS)rtJsonExternDefs.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(JSONSRCDIR)$(PS)rtJsonEncHexBinary.c


$(OBJDIR)$(PS)rtJsonEncIndent$(OBJ): $(JSONSRCDIR)$(PS)rtJsonEncIndent.c $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxErrCodes.h $(JSONSRCDIR)$(PS)osrtjson.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxCommon.h $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)rtxStack.h \
 $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxMemory.h $(RTXSRCDIR)$(PS)rtxBuffer.h \
 $(RTXSRCDIR)$(PS)rtxSList.h $(JSONSRCDIR)$(PS)rtJsonExternDefs.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(JSONSRCDIR)$(PS)rtJsonEncIndent.c


$(OBJDIR)$(PS)rtJsonEncInt$(OBJ): $(JSONSRCDIR)$(PS)rtJsonEncInt.c $(RTXSRCDIR)$(PS)rtxBuffer.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxSList.h $(RTXSRCDIR)$(PS)rtxCharStr.h \
 $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h $(JSONSRCDIR)$(PS)osrtjson.h \
 $(RTXSRCDIR)$(PS)rtxCommon.h $(RTXSRCDIR)$(PS)rtxMemory.h \
 $(JSONSRCDIR)$(PS)rtJsonExternDefs.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(JSONSRCDIR)$(PS)rtJsonEncInt.c


$(OBJDIR)$(PS)rtJsonEncInt64$(OBJ): $(JSONSRCDIR)$(PS)rtJsonEncInt64.c \
 $(JSONSRCDIR)$(PS)rtJsonEncInt.c $(RTXSRCDIR)$(PS)rtxBuffer.h $(RTXSRCDIR)$(PS)rtxContext.h \
 $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)rtxStack.h \
 $(RTXSRCDIR)$(PS)rtxSList.h $(RTXSRCDIR)$(PS)rtxCharStr.h $(RTXSRCDIR)$(PS)rtxError.h \
 $(RTXSRCDIR)$(PS)rtxErrCodes.h $(JSONSRCDIR)$(PS)osrtjson.h $(RTXSRCDIR)$(PS)rtxCommon.h \
 $(RTXSRCDIR)$(PS)rtxMemory.h $(JSONSRCDIR)$(PS)rtJsonExternDefs.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(JSONSRCDIR)$(PS)rtJsonEncInt64.c


$(OBJDIR)$(PS)rtJsonEncString$(OBJ): $(JSONSRCDIR)$(PS)rtJsonEncString.c $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxBuffer.h $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h \
 $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxSList.h \
 $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxErrCodes.h $(JSONSRCDIR)$(PS)osrtjson.h \
 $(RTXSRCDIR)$(PS)rtxCommon.h $(RTXSRCDIR)$(PS)rtxMemory.h \
 $(JSONSRCDIR)$(PS)rtJsonExternDefs.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(JSONSRCDIR)$(PS)rtJsonEncString.c


$(OBJDIR)$(PS)rtJsonEncUInt$(OBJ): $(JSONSRCDIR)$(PS)rtJsonEncUInt.c $(JSONSRCDIR)$(PS)rtJsonEncInt.c \
 $(RTXSRCDIR)$(PS)rtxBuffer.h $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h \
 $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)rtxStack.h \
 $(RTXSRCDIR)$(PS)rtxSList.h $(RTXSRCDIR)$(PS)rtxCharStr.h $(RTXSRCDIR)$(PS)rtxError.h \
 $(RTXSRCDIR)$(PS)rtxErrCodes.h $(JSONSRCDIR)$(PS)osrtjson.h $(RTXSRCDIR)$(PS)rtxCommon.h \
 $(RTXSRCDIR)$(PS)rtxMemory.h $(JSONSRCDIR)$(PS)rtJsonExternDefs.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(JSONSRCDIR)$(PS)rtJsonEncUInt.c


$(OBJDIR)$(PS)rtJsonEncUInt64$(OBJ): $(JSONSRCDIR)$(PS)rtJsonEncUInt64.c \
 $(JSONSRCDIR)$(PS)rtJsonEncInt.c $(RTXSRCDIR)$(PS)rtxBuffer.h $(RTXSRCDIR)$(PS)rtxContext.h \
 $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)rtxStack.h \
 $(RTXSRCDIR)$(PS)rtxSList.h $(RTXSRCDIR)$(PS)rtxCharStr.h $(RTXSRCDIR)$(PS)rtxError.h \
 $(RTXSRCDIR)$(PS)rtxErrCodes.h $(JSONSRCDIR)$(PS)osrtjson.h $(RTXSRCDIR)$(PS)rtxCommon.h \
 $(RTXSRCDIR)$(PS)rtxMemory.h $(JSONSRCDIR)$(PS)rtJsonExternDefs.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(JSONSRCDIR)$(PS)rtJsonEncUInt64.c


$(OBJDIR)$(PS)rtJsonEncUCS4String$(OBJ): $(JSONSRCDIR)$(PS)rtJsonEncUCS4String.c \
 $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)rtxErrCodes.h $(JSONSRCDIR)$(PS)osrtjson.h \
 $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxCommon.h $(RTXSRCDIR)$(PS)rtxContext.h \
 $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h \
 $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxMemory.h \
 $(RTXSRCDIR)$(PS)rtxBuffer.h $(RTXSRCDIR)$(PS)rtxSList.h \
 $(JSONSRCDIR)$(PS)rtJsonExternDefs.h $(JSONSRCDIR)$(PS)rtJsonEncUTF.c
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(JSONSRCDIR)$(PS)rtJsonEncUCS4String.c


$(OBJDIR)$(PS)rtJsonEncUnicode$(OBJ): $(JSONSRCDIR)$(PS)rtJsonEncUnicode.c $(RTXSRCDIR)$(PS)osMacros.h \
 $(RTXSRCDIR)$(PS)rtxErrCodes.h $(JSONSRCDIR)$(PS)osrtjson.h $(RTXSRCDIR)$(PS)osSysTypes.h \
 $(RTXSRCDIR)$(PS)rtxCommon.h $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h \
 $(RTXSRCDIR)$(PS)rtxExternDefs.h $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)rtxStack.h \
 $(RTXSRCDIR)$(PS)rtxError.h $(RTXSRCDIR)$(PS)rtxMemory.h $(RTXSRCDIR)$(PS)rtxBuffer.h \
 $(RTXSRCDIR)$(PS)rtxSList.h $(JSONSRCDIR)$(PS)rtJsonExternDefs.h \
 $(JSONSRCDIR)$(PS)rtJsonEncUTF.c
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(JSONSRCDIR)$(PS)rtJsonEncUnicode.c


$(OBJDIR)$(PS)rtJsonEncEdges$(OBJ): $(JSONSRCDIR)$(PS)rtJsonEncEdges.c $(JSONSRCDIR)$(PS)osrtjson.h \
 $(RTXSRCDIR)$(PS)osMacros.h $(RTXSRCDIR)$(PS)osSysTypes.h $(RTXSRCDIR)$(PS)rtxCommon.h \
 $(RTXSRCDIR)$(PS)rtxContext.h $(RTXSRCDIR)$(PS)rtxDList.h $(RTXSRCDIR)$(PS)rtxExternDefs.h \
 $(RTXSRCDIR)$(PS)rtxCommonDefs.h $(RTXSRCDIR)$(PS)rtxStack.h $(RTXSRCDIR)$(PS)rtxError.h \
 $(RTXSRCDIR)$(PS)rtxErrCodes.h $(RTXSRCDIR)$(PS)rtxMemory.h $(RTXSRCDIR)$(PS)rtxBuffer.h \
 $(RTXSRCDIR)$(PS)rtxSList.h $(JSONSRCDIR)$(PS)rtJsonExternDefs.h
	$(CC) $(CFLAGS) -c $(IPATHS) $(OBJOUT) $(JSONSRCDIR)$(PS)rtJsonEncEdges.c

