use strict;

# Execute all unit test programs

select (STDOUT);
$| = 1;
select (STDERR);
$| = 1;

my $makeprog;
if (defined ($ENV{'MAKEPROG'})) {
    $makeprog = $ENV{'MAKEPROG'};
}
elsif (defined ($ENV{'MSVCDIR'}) || defined ($ENV{'VCINSTALLDIR'})) {
    $makeprog = "nmake -nologo";
}
else {
    $makeprog = "make";
}

opendir (THISDIR, ".") || die ("unable to open directory");

while (my $filename = readdir (THISDIR)) {
    chomp $filename;
    if (-d $filename && $filename !~ /^\./) {
        chdir ($filename) || die "could not chdir to $filename";

        if (-e "makefile") {
            print ("\n$filename\n");
            `$makeprog clean`;
        }
        chdir ('..');
    }
    else {
        print ("$filename is not a directory\n");
    }
}

closedir (THISDIR);
