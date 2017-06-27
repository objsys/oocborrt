sub trFileSpec {
    local $filespec = shift;
    $filespec =~ s/\.\.\/rtsrc\//\$\(RTSRCDIR\)\$\(PS\)/g;
    $filespec =~ s/\.\.\/rtxsrc\//\$\(RTXSRCDIR\)\$\(PS\)/g;
    $filespec =~ s/\.\.\/rt3gppsrc\//\$\(RT3GPPSRCDIR\)\$\(PS\)/g;
    $filespec =~ s/\.\.\/rtbersrc\//\$\(BERSRCDIR\)\$\(PS\)/g;
    $filespec =~ s/\.\.\/rtcborsrc\//\$\(CBORSRCDIR\)\$\(PS\)/g;
    $filespec =~ s/\.\.\/rtjsonsrc\//\$\(JSONSRCDIR\)\$\(PS\)/g;
    $filespec =~ s/\.\.\/rtmdersrc\//\$\(MDERSRCDIR\)\$\(PS\)/g;
    $filespec =~ s/\.\.\/rtoersrc\//\$\(OERSRCDIR\)\$\(PS\)/g;
    $filespec =~ s/\.\.\/rtpersrc\//\$\(PERSRCDIR\)\$\(PS\)/g;
    $filespec =~ s/\.\.\/rtxersrc\//\$\(XERSRCDIR\)\$\(PS\)/g;
    $filespec =~ s/\.\.\/rtxmlsrc\//\$\(XMLSRCDIR\)\$\(PS\)/g;
    $filespec =~ s/\.\.\/libxml2src\/include\//$\(LIBXML2INC\)\$\(PS\)/g;
    $filespec =~ s/\.\.\/zlibsrc\//\$\(ZLIBSRCDIR\)\$\(PS\)/g;
    $filespec =~ s/\.\.\/expatsrc\//\$\(EXPATSRCDIR\)\$\(PS\)/g;
    return $filespec;
}

sub addRules {
    local $sourcefile = shift;
    local $objdir = shift;
    local $ccbasecmd = shift;
    local $cfilespec;
    local @rules = (`gcc $sourcefile -E -MM -I. -I.. -I../rtxsrc`);

    foreach $line (@rules) {
        # if line starts with a non-whitespace character, it is the 
        # start of a new rule ..
        if ($line =~ /^\w/) {
            # change .o to $(OBJ)
            $line =~ s/\.o/\$(OBJ)/;
            $line = '$(' . $objdir . ')$(PS)' . $line;

            # add a newline 
            print OUTFILE "\n";

            # get C source file from rule
            $cfilespec = trFileSpec ($sourcefile);
        }

        # change source file paths to use macro names
        $line = trFileSpec ($line);

        print OUTFILE $line;
    }

    # add compile command
    if ($cfilespec) {
        print OUTFILE ("\t$ccbasecmd -c " .
                       "\$(IPATHS) \$(OBJOUT) $cfilespec\n");
        print OUTFILE ("\n");
    }
}

# compare modification times on two files.
# result:
# <0 - file1 older than file2
# 0  - file 1 and 2 modification times are the same
# >0 - file2 older than file 1
sub cmpFileModTimes {
    local $file1 = shift;
    local $file2 = shift;
    local $mtime1 = (stat($file1))[9];
    local $mtime2 = (stat($file2))[9];
    return ($mtime2 - $mtime1);
}

sub checkDepends {
    local $rulesFile = shift;
    local $mtime;
    
    if (!defined($rulesFile) || $rulesFile eq "") {
        $rulesFile = "rules2.mk";
    }

    if (-e $rulesFile) {
        $mtime = (stat($rulesFile))[9];
        my $timestamp = localtime($mtime);
        print "$rulesFile mod time = $timestamp\n";
    }
    else { return 1; }

    opendir (SRCDIR, ".") ||
        die ("unable to open this directory");

    while ($filename = readdir (SRCDIR)) {
        # print "$filename\n";
        if ($filename =~ /\.[ch]$/ ||
            $filename =~ /\.cpp$/  ||
            $filename =~ /\.hh$/   ||
            $filename =~ /\.cc$/   ||
            $filename =~ /mkdepend\d?\.pl/ ||
            $filename eq 'objects.mk') {
            my $mtime2 = (stat("$filename"))[9];
            # print ("$filename : $mtime2\n");
            if ($mtime2 > $mtime) {
                print "$filename was modified after $rulesFile\n";
                return 1;
            }
        }
    }

    closedir (SRCDIR);

    return 0;
}

sub cd {
    local $dir = shift;
    print "chdir $dir\n";
    chdir ($dir) || die ("chdir $dir failed: $!");
}

1;
