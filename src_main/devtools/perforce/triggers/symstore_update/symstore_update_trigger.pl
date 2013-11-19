#!/usr/bin/perl -w
#
# Symbol Store Update trigger for Perforce
#
# Written by Milton Ngan 02/15/2007
#
# Takes a changelist number of the command line and 
# runs symstore to add the symbols to the symbol store 
# if it can find the file in the SymbolStoreUpdate.cfg file
#
# This script logs to ssu_log.txt and ssu_err_log.txt 
# in the same directory as the script

use P4;
use FindBin;
use File::Path;
use File::Basename;
use Time::HiRes qw(gettimeofday tv_interval);

use strict;
use warnings;

use constant SSU_CFG_FILE => 'SymbolStoreUpdate.cfg';
use constant SSU_TEMP_DIR => 'SymbolStoreUpdateTempFiles';
use constant LOG_INFO => 1;
use constant LOG_DEBUG => 2;
use constant LOG_TRACE => 3;

my $P4;

my %g_config = ();
my %g_fileHash = ();
my @g_dirList = ();
my $g_SymbolStoreDir;
my $g_nChanges = 0;
my $g_nUpdates = 0;

my $LOGGING_LEVEL = LOG_INFO;

$SIG{__DIE__} = sub {
    my $err = shift;
    my $time = scalar(localtime());
    die "$time: $err";
};

main();

sub logger
{
    my $logLevel = shift;

    if ($LOGGING_LEVEL >= $logLevel) 
			{
				my $time = scalar(localtime);
				print "$time: ", @_;
    }
}

sub LoadConfigFile
{
	open(my $cfgFile, SSU_CFG_FILE) ||
		die "Error loading config file: ".SSU_CFG_FILE."\n";
	while(<$cfgFile>) 
		{
			s/[\r\n]+$//; # trim off CR/LF
			s/^\s+//; # trim off leading white space
			s|^\s*//.*||; # Remove leading comments
			next unless /\S\s+\S/;
			my ($key, $val) = split(/\s+/, $_, 2);
			# strip crap off front and back of key/val
			$key =~ s/.*"(.*)".*/$1/; 
			$val =~ s/.*"(.*)".*/$1/; 

			logger LOG_TRACE, "CFG: $key => $val\n";
			push @{$g_config{lc $key}}, $val;
    }
}

sub ParseConfigFile
{
    LoadConfigFile();

    unless(exists($g_config{'symbolstore'})) 
			{
				die "Config file is missing 'SymbolStore' key\n";
			}
    $g_SymbolStoreDir = $g_config{'symbolstore'}[-1];
    
    if (exists($g_config{'p4binary'})) 
			{
				%g_fileHash = map { lc $_ => 1 } @{$g_config{'p4binary'}};
			}

    if (exists($g_config{'p4path'})) 
			{
				@g_dirList = map { s|\*|[^/]+|g; s#~#[^/]+\\.(?:exe|dll|pdb)#g; lc $_; } @{$g_config{'p4path'}};
			}

    if (!@g_dirList && !%g_fileHash) 
			{
				die "No 'path' or 'binary' specifications in config file\n";
			}
}

sub GetPerforceFileList 
{
	my $pattern = shift;
	my @files = $P4->Files($pattern);
	for my $file (@files) 
		{					
			$file->{depotFile} = lc $file->{depotFile};
			logger LOG_TRACE, "FILE: $file->{depotFile}\n";
		}

	return @files;
}

sub GetChangeList
{
    my $change = shift;
		my @changeList =  GetPerforceFileList("\@=$change");
		for my $change (@changeList) 
			{
				$g_nChanges++ if $change->{action} ne 'delete';
			}
		return @changeList;
}

sub GetPerforceFile
{
    my $p4File = shift;
    my $localFile = shift;
    my $changeNum = shift;
    $p4File =~ s/(?:\#\d+)?$/\@=$changeNum/ if $changeNum;
    
    my $localDir = dirname($localFile);
    mkpath($localDir);
    
    logger LOG_TRACE, "GET: $p4File\n";
		$P4->Print('-o', $localFile, $p4File);
}


sub UpdateSymStoreFile
{
    my $change = shift;
    my $file = shift;
    my $localFile = SSU_TEMP_DIR.$file;

    logger LOG_INFO, "Updating $file\n";
    GetPerforceFile($file, $localFile, $change);
    
    my $localDir = dirname($localFile);
    my $fileName = basename($localFile);

    logger LOG_DEBUG, "chdir $localDir\n";
    logger LOG_DEBUG, "symstore add /f \"$fileName\" /s \"$g_SymbolStoreDir\" /o /t SourceEngine\n";
    chdir $localDir;
    system("$FindBin::Bin/symstore.exe", 'add', '/f', $fileName, '/s', $g_SymbolStoreDir, '/t', 'SourceEngine', '/o');
    chdir $FindBin::Bin;

    $g_nUpdates++;
}

sub MatchesDir
{
	my $file = shift;

	for my $path (@g_dirList) 
		{
			if ($file =~ /$path/) 
				{
					logger LOG_TRACE, "PATH: $path\n";
					return 1;
				}
		}
	return 0;
}

sub UpdateSymStoreWithChangeList
{
    my $change = shift;
    my @changeList = GetChangeList($change);    

    for my $file (@changeList) 
			{
			  next if $file->{action} eq 'delete';
				logger LOG_TRACE, "TEST: $file->{depotFile}\n";
				if (exists($g_fileHash{$file})) 
					{
						logger LOG_TRACE, "FILE MATCH: $file->{depotFile}\n";
						UpdateSymStoreFile($change, $file->{depotFile});
					}
				elsif (MatchesDir($file->{depotFile})) 
					{
						logger LOG_TRACE, "DIR MATCH: $file->{depotFile}\n";
						UpdateSymStoreFile($change, $file->{depotFile});
					}
				else 
					{
						logger LOG_TRACE, "NO MATCH: $file->{depotFile}\n";
					}
			}
}

sub which 
{
   my $file = shift;

   logger LOG_DEBUG, "PATH = $ENV{PATH}\n";

   for my $dir (split(';', $ENV{PATH})) {
    if (-e "$dir/$file") 
			{
				logger LOG_DEBUG, "WHICH: $dir/$file\n";
			}
		 }
}

sub main
{
    chdir $FindBin::Bin;
    open(STDOUT, ">>ssu_log.txt");
    open(STDERR, ">>ssu_err_log.txt");

    logger LOG_INFO, "="x70, "\n";

    $ENV{PATH} = "$FindBin::Bin;$ENV{PATH}";
		$ENV{P4USER} ||= 'milton';

    my $change = shift @ARGV;
    die "No changelist given\n" unless $change;
    die "Invalid changelist: $change\n" unless $change =~ /^\d+/;

		$P4 = new P4;
		$P4->Tagged();
		$P4->Connect();

    ParseConfigFile();

    if (! -d SSU_TEMP_DIR) 
			{
				logger LOG_DEBUG, "Creating temp directory\n";
				mkpath(SSU_TEMP_DIR);
			}

    which("symstore.exe");
    which("dbghelp.dll");
    logger LOG_INFO, "Updating using change \@$change\n";

    UpdateSymStoreWithChangeList($change);    

    logger LOG_INFO, "$g_nChanges Change List Items\n";
    logger LOG_INFO, "$g_nUpdates Updated Items\n";
}
