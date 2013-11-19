$file = shift;
$path = $file;
$path =~ s/\\[^\\]*$//;
chdir $path;
system "p4 edit $file";
