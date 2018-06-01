while (<>) {
  chomp;
  if ($_ =~ /Version/) {
    @a = split(/ +/);
    print "$a[2]\n";
    exit;
  }
}
