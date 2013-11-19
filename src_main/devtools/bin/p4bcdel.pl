$file = shift;
$path = $file;
$path =~ s/\\[^\\]*$//;
chdir $path;
exec "p4 delete $file";

