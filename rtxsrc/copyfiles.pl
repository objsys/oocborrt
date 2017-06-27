my $header = <<'END_HEADER';
/**
 * Copyright (c) 2016-2017 by Objective Systems, Inc.
 * http://www.obj-sys.com
 *
 * This software is furnished under an open source license and may be
 * used and copied only in accordance with the terms of this license.
 * The text of the license may generally be found in the root
 * directory of this installation in the COPYING file.  It
 * can also be viewed online at the following URL:
 *
 *   http://www.obj-sys.com/open/lgpl2.html
 *
 * Any redistributions of this file including modified versions must
 * maintain this copyright notice.
 *
 *****************************************************************************/
END_HEADER

my @filenames = ( 
    "osMacros.h", "osSysTypes.h", 
    "rtxBigFloat.h", "rtxBigFloat.c", 
    "rtxBigInt.h", "rtxBigInt.hh", "rtxBigInt.c", "rtxBigIntCommon.c",
    "rtxBuffer.h", "rtxCommonDefs.h", 
    "rtxCharStr.h", "rtxCharStr.c", "rtxCtype.h", 
    "rtxDateTime.h", "rtxDateTime.hh", "rtxDateTime.c", 
    "rtxDateTimeCompare.c",
    "rtxDateTimeParse.c",
    "rtxDateTimeParseCommon.c",
    "rtxDateTimeToString.c",
    "rtxDateTimeToStringCommon.c",
    "rtxDList.h", "rtxDList.c", 
    "rtxError.h", "rtxError.c", "rtxErrCodes.h", "rtxExternDefs.h", 
    "rtxFile.h", "rtxGenValueType.c", "rtxGenValueType.h", 
    "rtxHexDump.c", "rtxHexDumpFile.c", "rtxHexDump.h", 
    "rtxIntStack.h", "rtxIntStack.c", 
    "rtxIntToCharStr.c", "rtxInt64ToCharStr.c", "rtxSizeToCharStr.c",
    "rtxPrint.h", "rtxPrint.c", 
    "rtxReal.h", "rtxRealHelper.c", 
    "rtxSList.h", "rtxSList.c", "rtxStack.h", "rtxStack.c", 
    "rtxUIntToCharStr.c", "rtxUInt64ToCharStr.c", 
    "rtxUTF8.h", "rtxUTF8.c"
    ) ;

# copy files and change headers to open source
foreach my $filename (@filenames) {
    &copyFile ($filename);
}

sub copyFile {
    local $filename = shift;
    open (INFILE, "../../rtxsrc/$filename") || 
        die ("could not open ../../rtxsrc/$filename");

    open (OUTFILE, ">./$filename")|| 
        die ("could not open ./$filename");

    # write open source header to outfile
    print OUTFILE $header;

    # skip header in input file
    while (<INFILE>) {
        s/\s+$//;
        last if (/\*\*\*\/$/);
    }

    # output reamining lines of file to target file
    my $opensrc = 1;
    my $liccheck = 0;
    while (<INFILE>) {
        next if (/LCHECK/);
        s/\s+$//;
        s/rtxContext\.hh/rtxContext.h/;
        s/OSCRTMALLOC0/malloc/g;
        s/OSCRTFREE0/free/g;
        s/^EXTRTMETHOD\s//;
        if ($_ =~ /\/\* START NOOSS \*\//) {
            $opensrc = 0; next;
        }
        elsif ($_ =~ /\/\* END NOOSS \*\//) {
            $opensrc = 1; next;
        }
        if ($_ =~ /\#ifndef _NO_LICENSE_CHECK/) {
            $liccheck = 1; next;
        }
        if ($liccheck) {
            $liccheck = 0 if ($_ =~ /\#endif/);
            next;
        }
        print OUTFILE "$_\n" if ($opensrc);
    }

    close (INFILE);
    close (OUTFILE);
}

# create dummy rtxMemLeakCheck.h file
open (OUTFILE, ">./rtxMemLeakCheck.h")|| 
    die ("could not open ./$rtxMemLeakCheck.h");

# write open source header to outfile
print OUTFILE $header;
print OUTFILE '#define DECLARE_MEMLEAK_DETECTOR' . "\n";

close OUTFILE;
