# generate dependency rules file (rules.mk) 
# usage: mkdepend [-v]

require "../rtxsrc/mkdepsubs.pl";

exit 0 if (&checkDepends("rules.mk") == 0);

$g_start = time();
$g_debug = 0;
$g_cfile = 0;

$numargs = @ARGV;

if ($numargs >= 1 && $ARGV[$numargs - 1] eq "-v") {
    $g_debug = 1;
}

sub genRulesFile {
    local $objectsFile = shift;
    local $rulesFile = shift;

    if (-e $rulesFile) {
        `rm -f ${rulesFile}~`;
        `mv $rulesFile ${rulesFile}~`;
    }

    open (OUTFILE, ">$rulesFile") || 
        die ("could not open $rulesFile for output");

    print OUTFILE "# C to object file compilation rules\n";

# open $objectsFile.mk file and use to get list of target objects

    open (INFILE, $objectsFile) || die ("could not open $objectsFile: $!");

    while (<INFILE>) {
        if (/\$\((\w+)\)\$\(PS\)(\w+)\$\(OBJ\)/) {
            $objdir = $1;
            $filebase = $2;
            # print "$objdir $filebase\n";

            # determine source file
            if ( -e "../rtxsrc/$filebase.c" ) {
                &addRules ("../rtxsrc/$filebase.c", $objdir, 
                           "\$(CC) \$(RTXCFLAGS)");
            }
            elsif ( -e "../rtxsrc/$filebase.cpp" ) {
                &addRules ("../rtxsrc/$filebase.cpp", $objdir, 
                           "\$(CCC) \$(RTXCCFLAGS)");
            }
            else {
                print "source file for $filebase not found\n";
            }
        }
    }

    close (INFILE);
    close (OUTFILE);
}

&genRulesFile ("objects.mk", "rules.mk");

if ($g_debug) {
    $g_end = time();
    $elapsed = $g_end - $g_start;
    print "elapsed time (seconds) = $elapsed\n";
}
