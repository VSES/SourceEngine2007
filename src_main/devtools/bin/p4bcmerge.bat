diff3 -E -m %3 %1 %2 > %4
splitdiff3.pl %4 %4 c:\theirs.tmp
bc2 %4 c:\theirs.tmp
