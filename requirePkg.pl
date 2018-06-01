$pkgname    = shift(@ARGV);
$site       = shift(@ARGV);
$version    = shift(@ARGV);
$pkg        = shift(@ARGV);
$configopt  = shift(@ARGV);
$configdis  = shift(@ARGV);
$dir = $pkg;
$dir =~ s/\.gz//g;
$dir =~ s/\.bz2//g;
$dir =~ s/\.tar//g;

print "\n";
print "MCS require the $pkgname package ($site, version $version or later).\n";
print "Either the package is not installed, or it is located in a non standard path.\n";
print "\n";
print " - If the package is not installed you can download it from the \n";
print "  reference site, and install it with the following commands:\n";
print "  (assuming the package name is $pkg)\n";
print "\n";
print "      tar xvzf $pkg\n";
print "      cd $dir\n";
print "      ./configure\n";
print "      make\n";
print "      make install (as root)\n";
print "\n";
print "\n";
print " - If the package is already installed in a non standard location you can\n";
print "   provide the correct path with:\n";
print "\n";
print "      ./configure $configopt=<PATH>\n";
print "\n";
print "\n";
if ($configdis) {
    print " - If you don't want the $pkgname facilities to be active type:\n";
    print "     ./configure $configdis\n";
}
