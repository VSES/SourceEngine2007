#!/usr/bin/perl -w
#
# usage: vtf_check.pl <changenum>
#
# Perforce trigger script to check that source textures and compiled
# VTF files are in sync
#
# This script assumes the general convention that the VTF files live
# in:
#
#    .../game/<mod>/materials/<subdir>/<base>.vtf
#
# and all the source textures live in the corresponding location under
# the content tree
#
#   .../content/<mod>/materialsrc/<subdir>/<base>.*
#

use FindBin;
use File::Path;
use File::Basename;
use Cwd 'abs_path';
use Time::HiRes qw(gettimeofday tv_interval);

use strict;

use constant LOG_INFO => 1;
use constant LOG_DEBUG => 2;
use constant LOG_TRACE => 3;

my $LOGGING_LEVEL = LOG_TRACE;

main();

sub logger
{
    my $logLevel = shift;

    if ($LOGGING_LEVEL >= $logLevel) {
	my $time = scalar(localtime);
	print LOGOUT "$time: ", @_;
	if ($logLevel == LOG_INFO) {
	    print @_;
	}
    }
}

sub GetPerforceFileList
{
    my $pattern = shift;
    my @fileList = ();

    if (open(my $p4pipe, "p4 files $pattern |")) {
	while(my $line = <$p4pipe>) {
	    chomp $line;
	    if ($line =~ /^(.+\#\d+) \- (?:edit|add) .+$/) {
		logger LOG_TRACE, "FILE: $pattern: $1\n";
		push @fileList, lc $1;
	    }
	}
    }
    else {
	warn "Failed to query pattern: $pattern\n";
    }

    return @fileList;
}


sub GetChangeList
{
    my $change = shift;
    my @changeList = GetPerforceFileList("\@=$change");

    return @changeList;
}

sub GetPerforceFile
{
    my $p4File = shift;
    my $localFile = shift;
    my $changeNum = shift;
    if ($changeNum) {
	$p4File =~ s/(?:\#\d+)?$/\@=$changeNum/;
    }
    
    my $localDir = dirname($localFile);
    mkpath($localDir);
    
    logger LOG_TRACE, "GET: $p4File\n";
    system('p4', 'print', '-q', '-o', $localFile, $p4File);
}

sub GetPerforceFiles {
    my $change = shift;
    my @files = @_;
    my @localFiles = ();

    for my $file (@files) {
	my $localFile = "temp/$file";
	$localFile =~ s/\#\d+$//;
	GetPerforceFile($file, $localFile, $change);
	push @localFiles, $localFile;
    }
    return @localFiles;
}

# CheckVTFSFile($)
# Args: local path to VTF file
# Returns: 1 if VTF matches sources, 0 otherwise
#
# Use the new CRC validation method first, then fall back to the more
# expesive diffsrc if the VTF isn't using the new format.
sub CheckVTFFile {
    my $file = shift;
    my $out = "";

    logger LOG_DEBUG, "CHECK: $file\n";
    which("vtex.exe");
    system("vtex.exe -nopause -nop4 -deducepath -crcvalidate $file >out.$$ 2>&1");
    if (open(PIPE, "out.$$")) {
	local $/ = undef;
	$out = <PIPE>;
	close(PIPE);
	unlink "out.$$";
    }
    logger LOG_DEBUG, "OUT: $out\n";

    if ($out =~ /CRC MISMATCH/s) {
	logger LOG_INFO, "CRC MISMATCH: $file\n";
	return 0;
    }
    elsif ($out =~ /OLD TEXTURE FORMAT/s) {
	logger LOG_INFO, "OLD TEXTURE: $file\n";
	system("call", "tx_diffsrc.cmd", $file);
	logger LOG_DEBUG, "DIFF TEXTURE: $file\n";
	return $? == 0;
    }    

    return 1;
}

sub CleanTempFiles {
    unlink(@_);
}

sub SetVProject {
    my $root = shift;

    logger LOG_DEBUG, "VPROJECT: $root\n";
    unless (-r "temp/$root/GameInfo.txt") {
	GetPerforceFiles(undef, "$root/GameInfo.txt");
    }

    $root = abs_path("temp/$root");
    $root =~ s|/|\\|g;
    $ENV{VPROJECT}=$root;
    $ENV{PATH}="$ENV{VPROJECT}\\..\\bin;$ENV{PATH}";
}

# CheckVTFFiles(@)
# Args: an array of Perforce filenames
# Returns: none
#
# Locates all the VTF files in the change list then checks them
# against the sources in the change list. If no sources are in the
# change list, then locate the latest ones in the depot and use those
# instead.
#
sub CheckVTFFiles {
    my $change = shift;
    my @files = @_;
    my @badfiles = ();

    my $oldpath = $ENV{PATH};
    for my $vtf (grep {m|/game/[^/]+/materials/.+/[^/]+\.vtf\#\d+$|} @files) {
	$vtf =~ s/\#\d+$//;
	my ($vtfFile) = GetPerforceFiles($change, $vtf);
	logger LOG_INFO, "VTF: $vtf\n";

	# .../game/<mod>/materials/<subdir>/<file>	
	my ($root, $mod, $subdir, $file) = ($vtf =~ m|^(.+)/game/([^/]+)/materials/(.+)/([^/]+)$|);
	logger LOG_DEBUG, "DECOMPOSE: $root, $mod, $subdir, $file\n";
	my $base = basename($file, '.vtf');
	
        # .../content/<mod>/materialsrc/<subdir>/<base>.???
	my @matches = grep {m|$root/content/$mod/materialsrc/$subdir/$base[^/]+$|} @files;
	my @tempFiles = ();

	if (@matches) {
	    logger LOG_DEBUG, "CHANGELIST MATCHES: ".scalar(@matches)."\n";
	    # Retrieve source textures from changelist
	    @tempFiles = GetPerforceFiles($change, @matches);
	}

	# Retrieve source textures from depot
	my $pattern = $vtf;
	$pattern =~ s|$root/game/$mod/materials/$subdir/$file|$root/content/$mod/materialsrc/$subdir/$base*|;
	logger LOG_DEBUG, "FIND IN DEPOT: $pattern\n";
	my @depotFiles = grep {
	    my $depotFile = $_;
	    $depotFile =~ s/\#\d+$//;
	    !grep {/^$depotFile\#\d+$/} @matches;
	}  GetPerforceFileList($pattern);
	if (@depotFiles) {
	    @tempFiles = GetPerforceFiles(undef, @depotFiles);
	}
	elsif (!@matches) {
	    push @badfiles, $vtf;
	    logger LOG_INFO, "NO TEXTURES IN DEPOT FOR VTF: $vtf\n";
	    logger LOG_INFO, "-"x70, "\n";
	    next;
	}

	$ENV{PATH} = $oldpath;
	SetVProject("$root/game/$mod");	
	unless (CheckVTFFile($vtfFile)) {
	    push @badfiles, $vtf;
	}
	CleanTempFiles($vtfFile, @tempFiles);

	logger LOG_INFO, "-"x70, "\n";
    }
    $ENV{PATH} = $oldpath;

    return @badfiles;
}

sub CheckTexFiles {
    my $change = shift;
    my @files = @_;
    my %texNames = ();
    my @badfiles = ();

    for my $tex (@files) {
	$tex =~ s/(?:[._]\d\d\d\d)?\.\w+\#\d+$//;
	if ($tex =~ m|^(.+)/content/([^/]+)/materialsrc/(.+)/([^/]+)$|) {
	    my ($root, $mod, $subdir, $base) = ($1, $2, $3, $4);
	    # Only check if we don't have a VTF in the change list
	    unless (grep {m|/game/$mod/materials/$subdir/$base\.vtf\#\d+|} @files) {
		$texNames{$tex} = [$root, $mod, $subdir, $base];
	    }
	}
    }

    my $oldpath = $ENV{PATH};
    for my $tex (sort keys %texNames) {
	logger LOG_INFO, "TEX: $tex\n";
	my ($root, $mod, $subdir, $base) = @{$texNames{$tex}};
	logger LOG_DEBUG, "DECOMPOSE: $root, $mod, $subdir, $base\n";

	my @matches = grep {m|^$tex.*|} @files;
	my @tempFiles = ();
	if (@matches) {
	    logger LOG_DEBUG, "CHANGELIST MATCHES: ".scalar(@matches)."\n";
	    # Retrieve source textures from changelist
	    @tempFiles = GetPerforceFiles($change, @matches);
	}
	
	my $pattern = $tex;
	$pattern =~ s|$root/content/$mod/materialsrc/$subdir/$base|$root/game/$mod/materials/$subdir/$base.vtf|;

	logger LOG_DEBUG, "FIND VTF: $pattern\n";
	my ($vtf) = GetPerforceFileList($pattern);
	if ($vtf) {
	    logger LOG_DEBUG, "FIND IN DEPOT: $tex*\n";
	    my @matsrc = grep {
		my $depotFile = $_;
		$depotFile =~ s/\#\d+$//;
		!grep {/^$depotFile\#\d+$/} @matches;
	    } GetPerforceFileList("$tex*");
	    my ($vtfFile, @depotFiles) = GetPerforceFiles(undef, $vtf, @matsrc);

	    $ENV{PATH} = $oldpath;
	    SetVProject("$root/game/$mod");
	    unless (CheckVTFFile($vtfFile)) {
		push @badfiles, $tex;
	    }
	    CleanTempFiles($vtfFile, @tempFiles, @depotFiles);
	}
	else {
	    logger LOG_DEBUG, "Skipping, no VTF Files Found: $tex\n";
	}
	logger LOG_INFO, "-"x70, "\n";
    }

    $ENV{PATH} = $oldpath;
    return @badfiles;
}


sub CheckTools {
    my @tools = grep {
	! -r "temp/$_";
    } qw(//valvegames/main/game/bin/tx_diffsrc.cmd 
         //valvegames/main/game/bin/vtfdiff.exe 
         //valvegames/main/game/bin/vtex.exe 
         //valvegames/main/game/bin/vtex_dll.dll
         //valvegames/main/game/bin/vstdlib.dll
         //valvegames/main/game/bin/Steam.dll
         //valvegames/main/game/bin/filesystem_stdio.dll
         //valvegames/main/game/bin/tier0.dll);

    logger LOG_DEBUG, "Fetching: ",join(", ", @tools),"\n";

    GetPerforceFiles(undef, @tools);
}

sub which 
{
    my $file = shift;

    logger LOG_DEBUG, "PATH = $ENV{PATH}\n";

    for my $dir (split(';', $ENV{PATH})) {
	if (-e "$dir\\$file") {
	    logger LOG_DEBUG, "WHICH: $dir\\$file\n";
	}
    }
}

sub main {
    my $root = $FindBin::Bin;
    chdir $root;
    $root =~ s|/|\\|g;
    $ENV{PATH} = "U:\\P4ROOT;$ENV{SystemRoot}\\system32";
    $ENV{P4USER} ||= 'milton';
    #$ENV{P4PORT} = "p4backup:1666";

    my $change = shift @ARGV;
    die "No changelist given\n" unless $change;
    die "Invalid changelist: $change\n" unless $change =~ /^\d+/;

    open(LOGOUT, ">>vtf_check.txt");
    open(STDERR, ">>vtf_check_err.txt");
    
    my $start = [gettimeofday];

    logger LOG_INFO, "="x70, "\n";

    CheckTools();

    logger LOG_INFO, "Change List: $change\n";
    my @files = GetChangeList($change);
    my @badvtfs = CheckVTFFiles($change, @files);
    my @badtexs = CheckTexFiles($change, @files);

    my $stop = [gettimeofday];

    logger LOG_INFO, scalar(@files)." Change List Items\n";
    logger LOG_INFO, scalar(@badvtfs)." Bad VTF Files\n";    
    logger LOG_INFO, scalar(@badtexs)." Bad Tex Files\n";    
    logger LOG_INFO, "Check took ", sprintf("%.1f secs", tv_interval($start, $stop)), "\n";
    
    if (@badvtfs || @badtexs) {
	logger LOG_INFO, "-"x70, "\n";
	for my $file (@badvtfs,@badtexs) {
	    logger LOG_INFO, "$file\n";
	}
    }
    
    logger LOG_INFO, "="x70, "\n";
    
    my $n_errors = scalar(@badvtfs)+scalar(@badtexs);
    exit($n_errors);
}
