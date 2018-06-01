while (<>) {
  chomp;
  @a = split(/ => /);
  $b = $a[1];
  foreach $a (split(/:/, $b)) {
    if ($a =~ m!^/!) {
      print "$a\n";
      exit;
    }
  }
}
print "./\n";
