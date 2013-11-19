#!/usr/bin/perl -w
#
# Symbol Store Update trigger for Perforce
#
# Written by Mike Durand 05/14/2007
#
# Takes a changelist number on the command line and 
# checks all of the DLLs and EXEs included in that
# changelist for a digital signature. If there are binaries
# with no digital signature then the script returns 1.
#
# This script logs to sigcheck_log.txt and sigcheck_err_log.txt 
# in the same directory as the script

use FindBin;
use File::Path;
use File::Basename;
use Time::HiRes qw(gettimeofday tv_interval);

use strict;
use warnings;

use constant SIGCHECK_TEMP_DIR => 'SignatureCheckTempFiles';
use constant LOG_INFO => 1;
use constant LOG_DEBUG => 2;
use constant LOG_TRACE => 3;

my $g_messages = "Submit Failed:\n\n";
my $g_returnCode = 0;

my $LOGGING_LEVEL = 1;

$SIG{__DIE__} = sub {
    my $err = shift;
    my $time = scalar(localtime());
    die "$time: $err";
};

main();

sub logger
{
    my $logLevel = shift;

    if ($LOGGING_LEVEL >= $logLevel) {
	my $time = scalar(localtime);
	print "$time: ", @_;
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
    print "p4 print -q -o $localFile, $p4File\n";
    system('p4', 'print', '-q', '-o', $localFile, $p4File);
}

sub CheckSignatureOfFile
{
    my $change = shift;
    my $file = shift;
    my $localFile = SIGCHECK_TEMP_DIR.$file;

    GetPerforceFile($file, $localFile, $change);
    logger LOG_TRACE, "UPDATE: $localFile\n";
    
    my $localDir = dirname($localFile);
    my $fileName = basename($localFile);

    chdir $localDir;
    system("$FindBin::Bin/signtool.exe", 'verify', '/q', '/pa', $fileName);
	my $exit_value  = $? >> 8;
	if ($exit_value) {
	  $g_returnCode = 1;
	  $g_messages = "$g_messages $file has not been signed.\n";
	}

    # Delete the file now that we have checked the signature
    unlink($fileName);
    chdir $FindBin::Bin;
}

sub CheckSignatureOfFilesWithChangeList
{
    my $change = shift;
    my @changeList = GetChangeList($change);    

    for my $file (@changeList) {
	  if ((basename($file) =~ /\.dll/) || (basename($file) =~ /\.exe/) || (basename($file) =~ /\.msi/)) {
        print "Checking signature of $file in changelist $change\n";
	    CheckSignatureOfFile($change, $file);
	  }
	  else {
	    print "Not checking $file\n";
	  }
    }
}

sub which 
{
   my $file = shift;

   logger LOG_DEBUG, "PATH = $ENV{PATH}\n";

   for my $dir (split(';', $ENV{PATH})) {
    if (-e "$dir/$file") {
	logger LOG_DEBUG, "WHICH: $dir/$file\n";
    }
   }
}

sub main
{
    chdir $FindBin::Bin;

    # Remove temp directory and log files
    rmdir(SIGCHECK_TEMP_DIR);
    unlink("sigcheck_err_log.txt");
    unlink("sigcheck_log.txt");
	
    # Redirect SDTOUT and STDERR so that the P4's and signtool's output doesn't show up in the trigger failure text
    open(OLDOUT, ">&STDOUT");
    open(OLDERR, ">&STDERR");
    open(STDOUT, ">>sigcheck_log.txt");
    open(STDERR, ">>sigcheck_err_log.txt");

    logger LOG_INFO, "="x70, "\n";

    $ENV{PATH} = "$FindBin::Bin;$ENV{PATH}";
    $ENV{P4USER} = 'milton';

    which("signtool.exe");

    my $change = shift @ARGV;
    die "No changelist given\n" unless $change;
    die "Invalid changelist: $change\n" unless $change =~ /^\d+/;
    CheckSignatureOfFilesWithChangeList($change);    
	
    # Re-enable SDTOUT and STDERR so that the message output shows up in the trigger failure text
    close(STDOUT);
    close(STDERR);
    open(STDOUT, ">&OLDOUT");
    open(STDERR, ">&OLDERR");
	
    if ($g_returnCode) {
      $g_messages = "$g_messages \nPlease digitally sign these files using 'vsign.bat' and try your submission again.\nYou can find out more about using 'vsign.bat' here: http://intranet.valvesoftware.com/wiki/index.php/Digitally_Signing_Binaries.\n";
      print $g_messages;
    }

    # Close all files
    close(STDOUT);
    close(STDERR);
    close(OLDOUT);
    close(OLDERR);

    exit $g_returnCode;
}
