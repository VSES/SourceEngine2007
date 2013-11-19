#!perl
use XML::Simple;
use Data::Dumper;
use Getopt::Long;
use File::Basename;
use Cwd 'abs_path';
use Cwd;
use Digest::MD5  qw(md5 md5_hex md5_base64);


GetOptions( "verbose"=>\$verbose,
			"x360"=>\$x360 );

$sln_name=shift || &PrintArgumentSummaryAndExit;

my $curdir=cwd;

$curdir=~ (m@(^.*/src)@i) || die "Can't determine srcroot from current directory $curdir";

$srcroot=lc($1);

$linker_tool_name="VCLinkerTool";
$linker_tool_name="VCX360LinkerTool" if ($x360);

&ReadVPCProjects;
if ( $sln_name =~ /^\@(\S+)/)
{
	$sln_name=$1;
	&ReadGroup($1);
	foreach $proj (@PROJS)
	{
		&AddProject(lc(abs_path($proj)),1);
	}
	&WriteSolutionFile($sln_name);
}
else
{
	# normal mode
	while($_ = shift )
	{
	  if ( /^\@(\S+)/)
		{
		  # accept group names
		  &ReadGroup($1);
		  foreach $proj (@PROJS)
			{
			  &AddProject(lc(abs_path($proj)),1);
			}
		}
	  elsif ( /^\*(\S+)/)
		{
		  $output_only_projects_dependent_upon = lc( $1 );
		  &ReadGroup("everything");
		  foreach $proj (@PROJS)
			{
			  &AddProject(lc(abs_path($proj)),0);
			}
		}
	  else
		{
		  unless(/\.vcproj/)		# if no extension specified, assume its a project name from projects.vgc
			{
			  $_=$projpath{lc($_)} if length($projpath{lc($_)});
			}
		  foreach $path (split(/\s+/,$_))
			{
			  $path=~s@\s+@@g;
			  &AddProject(lc(abs_path($path)),1) if length($path);
			}
		}
	}
	&WriteSolutionFile($sln_name);
}


sub WriteSolutionFile
{
	local($sln)=@_;
	$sln="$sln.sln" unless ( $sln=~/\./); # add extension if needed
	if ( ( -e $sln && ( ! ( -w $sln ) ) ) )
	{
		print STDERR "$sln is write-protected. Doing p4 edit.\n";
		print `p4 edit $sln`;
		die "Failed to make $sln writeable" if ( ! ( -w $sln ) );
	}
	open(SLN,">$sln" ) || die "can't open output $sln";
	# generate a guid for the sln
	my $sln_guid="8BC9CEB8-8B4A-11D0-8D11-".substr(uc(md5_hex(basename($sln))),0,12);

	print SLN "\xef\xbb\xbf\nMicrosoft Visual Studio Solution File, Format Version 9.00\n# Visual Studio 2005\n";
	foreach $proj (@PROJECTS)
	{
	  # check dependencies for "*" projects
	  if ( (! length( $force_project_inclusion{$proj} ) ) &&
		   ( length($output_only_projects_dependent_upon) ))
		{
		  my $skip_it = 1;
		  $skip_it = 0 if ( lc( $output_only_projects_dependent_upon ) eq lc( $proj ) );
		  foreach $lib (split(/,/,$depends_on{$proj}))
			{
			  $skip_it = 0 if ( $output_only_projects_dependent_upon eq lc($lib) );
			  foreach $prvd (split(/,/,$provider{$lib}))
				{
				  $skip_it = 0 if ( $output_only_projects_dependent_upon eq $prvd );
				}
			}
		  next if ( $skip_it );
		}

	  print SLN "Project(\"{",$sln_guid,"}\") = \"$proj\", \"$relpath{$proj}\", \"$guid{$proj}\"\n";
	  
	  #, now do dependencies
	  if ( length($depends_on{$proj} ) )
		{
		  print SLN "\tProjectSection(ProjectDependencies) = postProject\n";
		  foreach $lib (split(/,/,$depends_on{$proj}))
			{
			  if ( length($provider{$lib}) )
				{
				  foreach $prvd (split(/,/,$provider{$lib}))
					{
					  print SLN "\t\t$guid{$prvd} = $guid{$prvd}\n" if ( length($prvd) );
					}
				}
			  else
				{
				  print "I don't know who provides $lib for $proj\n" if ( $verbose && length($lib) );
				}
			  
			}
		  print SLN "\tEndProjectSection\n";
		}
	  print SLN "EndProject\n";
	}
	
	print SLN "Global\n";
	print SLN "\tGlobalSection(SolutionProperties) = preSolution\n";
	print SLN "\t\tHideSolutionNode = FALSE\n";
	print SLN "\tEndGlobalSection\n";
	print SLN "EndGlobal\n";
}

sub PrintArgumentSummaryAndExit
{
	print "Format of command is\n";
	my $switches="[ -verbose -x360 ]";
	print "\t MKSLN $switches <solutionname.sln > proj1.vcproj proj2.vcproj ...\n";
	print "OR\t MKSLN $switches \@vpcgroupname\n";
	print "OR\t MKSLN $switches sln_name \@vpcgroupname\n";
	print "OR\t MKSLN $switches sln_name *project create a solution including only that project and things dependent on it.\n";
}


sub AddProject
{
	local($fname, $force )=@_;
	local($/);
	print "add project $fname\n" if ( $verbose );
	open( VCP_IN, $fname ) || die "can't open $fname";
	my $xmltext=<VCP_IN>;
	close VCP_IN;
	my $xml=XMLin($xmltext, forcearray => [ 'File', 'Filter' ] ); #, keyattr =>[ 'name', 'key', 'id', 'Name']);
	my $pname=lc($xml->{Name});
	$force_project_inclusion{$pname} = "yes" if ( $force );
	return if ($already_processed{$pname} );
	$already_processed{$pname}=1;
	my $id=$xml->{ProjectGUID};
	unless( length($id) )
	{
		# none in the project file. make one.
		$id="{AAAAAAAA-8B4A-11D0-8D11-".substr(uc(md5_hex($pname)),0,12)."}";
		# this sucks. no way to set dependencies in a newly generated solution file if the projects don't have guids set,
		# which only happens when you read them into visual studio the first time. so i'll rewrite the vpcroj
		AddGUIDToVCProj($fname,$id);

	}
	$id = "{".$id."}" unless( $id=~ /}/);
	$guid{$pname}=$id;
	push @PROJECTS,$pname;
	$vcprojpath{$pname}=$fname;

	#get targetname
	my $targetname=$xml->{Name};
	# get output target

	my $tools=$xml->{Configurations}->{Configuration}[0];
	# walk the tool list to see if this project outpus a .lib that something might depend on
	my $outputtarget;
	foreach $tool (@{$tools->{'Tool'}})
	{
		if ( $tool->{Name} eq "VCLibrarianTool" )
		{
			my $outputtarget=lc(basename($tool->{OutputFile}));
			$provider{$outputtarget}.=",$pname";
			print "$pname provides $outputtarget\n" if ($verbose);
		}
		if ( $tool->{Name} eq $linker_tool_name )
		  {
			my $outputtarget=$tool->{'ImportLibrary'};
			if ( length($outputtarget) )
			  {
				$outputtarget=~s/\$\(TargetName\)/$targetname/i;
				$outputtarget=lc(basename($outputtarget));
				$outputtarget =~ s/\.lib/_360.lib/ if ( $x360 );
				$provider{$outputtarget}=basename($pname);
				print "$pname provides $outputtarget\n" if ($verbose);
			  }
		  }
	}

	foreach $filter (@{$xml->{Files}->{Filter}})
	{
		foreach $file (@{$filter->{File}})
		{
			my $f = lc($file->{RelativePath});
			if ( $f=~/\.lib$/i) # library dependency
			{
			  my $libname=basename($f);
			  $depends_on{$pname}.=",".$libname;
			  print "$pname depends on $libname\n" if ($verbose);
			}
		}
	}
	# generate relative pathname
	$fname=~s@^$srcroot/@@i;
	$fname=~s@/@\\@g;
	$relpath{$pname}=$fname;
	
}

sub ReadGroup
{
	local($matchgroup)=@_;
	my $curmatch=0;
	open(GROUPS,"$srcroot/vpc_scripts/groups.vgc") || die "can't open groups.vgc";
	while(<GROUPS>)
	{
		&FixupVPCLine;
		if (/^\$Group\s+(.*)$/)
		{
			my $groups=" $1 ";
			$groups=~s@\"@@g;
			$curmatch=0;
			$curmatch=1 if ( $groups=~/ $matchgroup /i );
		}
		elsif ( $curmatch && (/^\s*\"([^\"]+)\"/) )
		{
			my $proj=lc($1);
			my $path=$projpath{$proj};
			if (length($path))
			{
			  foreach $prj (split(/\s+/, $path))
				{
				  $prj=~s@\s+@@g;
				  next unless (length($prj));
				  if ( -e $prj )
					{
					  push @PROJS,$prj;
					  print "found proj $prj\n" if ($verbose);
					}
				  else
					{
					  print STDERR "can't find $prj\n";
					}

				}
			}
			else
			{
				print STDERR "couldn't find project name $proj (group = $matchgroup )\n";
			}
		}
		else
		{
			$curmatch = 0 if (/\}/);
		}			

	}
}

sub ReadVPCProjects
{
	# group mode. ugh 100x harder to parse vpc than .vcproj
	open(PROJS,"$srcroot/vpc_scripts/projects.vgc" ) || die "can't open projects.vgc";
	while(<PROJS>)
	{
		&FixupVPCLine;
		if (/^\s*\$Project\s+\"([^\"]+)\"/)
		{
			$curproj=$1;
		}
		elsif (/^\s*\"([^\"]+)\.vpc\"/)
		{
			my $base = $1;
			$base="$base"."_x360" if ( $x360 );
			$projpath{lc($curproj)}.=" $base.vcproj";
		}
	}
	close PROJS;
}

sub FixupVPCLine
{
	s@[\n\r]@@g;
	s@//.*$@@g;				# kill comments

	# use [] skips. need something smarter here. for now, implicit /allgames except hl1
	$_=undef if ( /\$HL1/);
}

sub AddGUIDToVCProj
{
	# need to add guid. we will do it with line by line parsing instead of xml parsing, since visual studio is picky about its xml
	local($fname,$id)=@_;
	local($_);
	my @Lines, $didit=0;
	local($/)="\n";
	open( VCP_IN, $fname ) || die "can't open $fname";
	while( <VCP_IN> )
	{
		push @Lines, $_;
	}
	close VCP_IN;
	print STDERR "$fname has never been loaded and lacks a guid. I will add one\n" if ($verbose);
	open(VCPOUT,">$fname") || die "can't write to vcproj $fname";
	foreach $_ (@Lines)
	{
		print VCPOUT "$_";
		if ( (/^(\s+)Name=/) && (! $didit ))
		{
			$didit=1;
			print VCPOUT "$1ProjectGUID=\"$id\"\n";
		}
	}
	close VCPOUT;
}


