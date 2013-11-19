use Cwd;

my $dir = getcwd;
 
chdir "../../materialsystem/stdshaders";
 
@output = `perl ..\\..\\devtools\\bin\\checkshaderchecksums.pl stdshader_dx9_20b.txt`;
$output = join "", @output;
@output = `perl ..\\..\\devtools\\bin\\checkshaderchecksums.pl stdshader_dx9_30.txt`;
$output .= join "", @output;

chdir $dir;

print $output;

if( length( $output ) > 0 )
{
	print "writing errors.txt\n";
	open FP, ">errors.txt";
	print FP "$output";
	close FP;
}
