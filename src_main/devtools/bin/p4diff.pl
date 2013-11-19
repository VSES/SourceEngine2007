# To install:
#	1) Make sure bc2 (Beyond Compare) is in your path
#	2) Go to Tools->Customize in VC6 and click on Tools.
#	3) Add a new command "p4diff" with the following properties:
#		Command: perl.exe
#		Arguments: p4diff.pl $(FilePath)
#		Initial Directory: (path to devtools\bin, ie. c:\hl2\src_main\devtools\bin
#		NOTE: You can add a preference for which side in the diff the local file is on
#		as an optional param, ie: "p4diff.pl $(FilePath) left" or "p4diff.pl $(FilePath) right"
#	4) Check "Close window on exit"
#	5) Check "Use output window" to put the file history into the output pane
#	
# Now, to compare the file htat you are currently editing in your local branch against
# the last main version that you integrated/copied from, do Tools->p4diff.
#
# (I'm sure there is a trivial way to bind this to a key, but I haven't done that yet.)		
#
# You can also add a toolbar button for this:
#	1) Look in the tools menu to see which numbered user tool you are (the small number next to the hammer icon.)
#	2) Go to Tools->Customize->Commands
# 	3) Select Category "Tools"
#	4) Drag the appropriate icon (in my case the hammer with the 8 next to it) to your toolbar.
#
# If you want to bind a key to this command:
#	1) Go to tools->customize->Keyboard
#	2) Select Category "Tools"
#	3) Choose "User Tool 8" (or whatever user tool is appropriate as above in the instructions for the adding a toolbar button.)  
#	4) Assign to a key.


$file = shift;
$leftorright = shift;
$dir = $file;
$dir =~ s,\\[^\\]*$,,;
chdir $dir;
@history = `p4 filelog $file`;
print @history;
foreach $line (@history)
{
	if( $line =~ m,(merge|copy|branch) from (//ValveGames/main/src/.*)\#(\d+)$,i )
	{
		$mainfile = $2;
		$mainversion = $3;
		last;
	}
}

die "Can't tell what version you last merged/copied from!\n" if !defined $mainfile;

print "mainfile: $mainfile mainversion: $mainversion\n";

$cmd = "p4 print $mainfile#" . $mainversion;
@data = `$cmd`;

shift @data;

open TMP, ">c:\\mainfile.tmp";
print TMP @data;
close TMP;

if( $leftorright eq "right" )
{
	system "bc2 c:\\mainfile.tmp $file";
}
else
{
	system "bc2 $file c:\\mainfile.tmp";
}
