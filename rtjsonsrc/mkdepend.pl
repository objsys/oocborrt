# generate dependency rules file (rules.mk) 
# usage: mkdepend [-v]

require "../../rtxsrc/mkdepsubs.pl";

if (0 == &checkDepends("rules.mk")) {
    &cd ('../rtxsrc');
    if (&checkDepends("rules.mk") != 0) {
        print "invoke rtxsrc/mkdepend.pl..\n";
        `perl mkdepend.pl`;
    }
    else {
        my $res = &cmpFileModTimes('../rtxsrc/rules.mk', 
                                   '../rtjsonsrc/rules.mk');
        print ("cmpFileModTimes result = $res\n");
        exit if ($res >= 0);
    }
    &cd ('../rtjsonsrc');
}

if (-e "rules.mk") {
    `rm -f rules.mk~`;
    `mv rules.mk rules.mk~`;
}

open (OUTFILE, ">rules.mk") || 
    die ("could not open rules.mk for output");

print OUTFILE "# C to object file compilation rules\n";

# open objects.mk file and use to get list of target objects

open (INFILE, "objects.mk") || die ("could not open objects.mk: $!");

while (<INFILE>) {
    if (/\$\((\w+)\)\$\(PS\)(\w+)\$\(OBJ\)/) {
        $objdir = $1;
        $filebase = $2;
        # print "$objdir $filebase\n";

        # determine source file
        if ( -e "../rtjsonsrc/$filebase.c" ) {
            &addRules ("../rtjsonsrc/$filebase.c", $objdir, 
                       "\$(CC) \$(CFLAGS)");
        }
        elsif ( -e "../rtjsonsrc/$filebase.cpp" ) {
            &addRules ("../rtjsonsrc/$filebase.cpp", $objdir, 
                       "\$(CCC) \$(CFLAGS)");
        }
        else {
            print "source file for $filebase not found\n";
        }
    }
}

close (INFILE);
close (OUTFILE);
