# Execute all unit test programs

select (STDOUT);
$| = 1;
select (STDERR);
$| = 1;

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

while ($filename = readdir (THISDIR)) {
    chomp ($filename);
    print ("\n$filename\n");
    if (-d $filename && $filename !~ /^\./) {
        print ("chdir $filename\n");
        chdir ($filename) || die "could not chdir to $filename";

        if (-e "makefile") {
            print ("\n$filename\n");
            `$makeprog`;

            if (-e "unittest.exe" || -e "unittest") {
                print ("executing unittest ..\n");
                print `unittest`;
            }
        }
        chdir ('..');
    }
}

closedir (THISDIR);
