#! perl

# simple p4 mutex mgr


use Win32::GUI;
use Win32::Console;
use Win32::Process;



# config vars
$msgdir="//fileserver/user/perforce";

# p4 must be in the path
undef $puser,$p4client;

# lets find the p4 user
open(ENV_VARS,"p4 set|") || die "can't run p4 set command";
while(<ENV_VARS>)
  {
	$p4user=$1 if (/^P4USER=(\S+)/i);
	$p4client=$1 if (/^P4CLIENT=(\S+)/i);
  }

close ENV_VARS;

die "can't find p4user" unless length($p4user);

$p4user=lc($p4user);

# now, lets figure out what they want to do

# first lets take a stab at guessing the branch
$branch="main_src";
$_=$p4client;
$_=`cd` unless (length($_));

$branch="rel_src" if (/rel/i);
$branch="main_src" if (/main/i);
$branch="staging_src" if (/staging/i);


$cmd=lc(shift);
$cmd="monitor" unless length($cmd);


$maybe_branch=shift;
$branch=$maybe_branch if (length($maybe_branch));

$maybe_user=shift;
$p4user=lc($maybe_user) if length($maybe_user);

if ($cmd eq "lock")
  {
	&get_status;
	if ($lock_free )
	  {
		exit if ( &get_lock )
	  }
	&queue_lock;
	&launch_monitor_detached;
  }
elsif ($cmd eq "monitor")
  {
	&launch_monitor_detached;
  }
elsif ($cmd eq "monitorquiet")
  {
	&monitor;
  }
elsif ($cmd eq "release")
  {
	&release_lock;
  }
elsif ($cmd eq "yield")
  {
	&yield_lock;
  }
elsif ($cmd eq "leave")
  {
	&leave_queue;
  }
elsif ($cmd eq "query")
  {
	&get_status;
	&print_status;
  }
else
  {
	die "unrecognized command $cmd";
  }

sub find_devtools_perfoce
{
  local($_)=$ENV{"VProject"};
  s@\\game\\.*$@@i;
  if (-e "$_/src/devtools/perforce/p4lock.pl")
	{
	  return "$_/src/devtools/perforce";
	}
  foreach $_ (split(/;/,$ENV{"Path"})) # vproject didn't work, look for devtools on path
	{
	  if ((s@src\\devtools\\.*@src\\devtools\\perforce@i) && (-e "$_/p4lock.pl"))
		{
		  return $_;
		}
	}
  return "";
}

sub launch_monitor_detached
{
  # if we can figure out where perl and the script are, we can run
  # ourselves again, detached, so we won't leave a console dangling
  my $pdir=&find_devtools_perfoce;
  if (length($pdir))
	{
	  $pdir =~ s@/@\\@g;
#	  print STDERR "found devtools at $pdir\n";
	  Win32::Process::Create($ProcessObj,
							 $^X,
							 "perl $pdir/p4lock.pl monitorquiet $branch $p4user",
							 0,
							 NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW,
							 ".")|| die ErrorReport();
	}
  else
	{
	  &monitor;
	}
}


sub print_status
{
  foreach $_ ( keys(%lockers) )
	{
	  print "$_ has the lock\n" unless( $_ eq $p4user );
	  print "YOU have the lock\n" if( $_ eq $p4user );
	}
  my $count=1;
  foreach $_ (sort { $waiters{$a} <=> $waiters{$b} } keys(%waiters))
	{
	  print "$count : $_ has been in the queue since ",&ptime($waiters{$_}),".\n";
	  $count++;
	}
  print "The lock is FREE" if ($lock_free);
}

sub get_status
  {
	undef %lockers;
	undef %waiters;
	$lock_free=1;
	$counters=`p4 counters`;
	my $lockname=$branch."_lock_";
	my $waitname=$branch."_wait_";
	my $hadname=$branch."_got_time_";
	my $am_lock_owner;
	my $lowest_waiter;
	foreach(split(/\n/,$counters))
	  {
		if (/$hadname(.*)\s+=\s*(\d+)/i)
		  {
			$hadtime{$1}=$2;
		  }
		if (/$lockname(.*)\s+=\s*(\d+)/i)
		  {
			if ($2>0)
			  {
				$lockers{$1}=1;
				undef $lock_free;
				$am_lock_owner = 1 if (lc($1) eq $p4user);
			  }
			
		  }
		if ( (/$waitname(.*)\s+=\s*(\d+)/i) && ($2 > 0 ))
		  {
			if ((lc($1) eq $p4user) && ($am_lock_owner))
			  {
				# we're the owner AND in the queue?
				print "we're the owner AND in the queue?\n";
				$cmd="p4 counter $branch"."_wait_$p4user 0";
				print `$cmd`;
			  }
			else
			  {
				$waiters{$1}=$2;
				$lowest_waiter=$1 if ( (length($lowest_waiter)==0) || ( $waiters{$lowest_waiter}>$2) );
			  }
		  }
	  }
	if ($am_lock_owner && (! $g_was_lock_owner) )
	  {
		$g_got_lock_time=time;
		# play a sound. use eval so won't bomb if no Win32::Sound
		eval
		  {
			use Win32::Sound;
			if (-e "$msgdir/gotlock.wav")
			  {
				Win32::Sound::Play("$msgdir/gotlock.wav",SND_ASYNC);
			  }
			else
			  {
				Win32::Sound::Play(SystemExclamation,SND_ASYNC);
			  }
		  };
		eval
		  {
			use Win32::API;
			my $FlashWindowEx = Win32::API->new("user32","FlashWindowEx", "P", "L");
			my $parms=pack("LLLLL",20,$main_window->{-handle},2,30,500);
			$FlashWindowEx->Call($parms);
		  };
		eval
		  {
			if( defined $ENV{"P4LOCK_EMAIL"} )
			{
				my $email = $ENV{"P4LOCK_EMAIL"};
				use Net::SMTP;

				$smtp = Net::SMTP->new('exchange2.valvesoftware.com');

				$smtp->mail("p4lock.pl");
				$smtp->to("$email");

				$smtp->data();
				$smtp->datasend("To: $email\n");
				$smtp->datasend("Subject: You have the lock!<EOM>\n");
				$smtp->dataend();

				$smtp->quit;
			}
		  };
	  }

	$g_was_lock_owner=$am_lock_owner;
	if ( $lowest_waiter ne $last_lowest_waiter)
	  {
		$last_good_status = time;
		$last_lowest_waiter = $lowest_waiter;
	  }
	if ( $lock_free && ($lowest_waiter) )
	  {
		# check for zombies - people at the head of the queue who refuse to grab the lock.
		if (time-$last_good_status > 5*60) # five minutes
		  {
			print "dequeue zombie $lowest_waiter\n";
			&clearmsg($lowest_waiter);
			$cmd="p4 counter $branch"."_wait_$lowest_waiter 0";
			`$cmd`;
			$last_good_status=time;
		  }
	  }
	else
	  {
		$last_good_status=time;
	  }

	if ($gui_setup)
	  {
		if ($lowest_waiter && ($lowest_waiter eq $p4user) )
		  {
			$refresh_timer->Interval(20000); # every 20 secs if we are next
		  }
		else
		  {
			$refresh_timer->Interval(60000); # every 60 secs if we are not
		  }
	  }
	undef $lock_free if ( $lowest_waiter && ( $lowest_waiter ne $p4user ));
  }

sub release_lock
  {
	&get_status;
	if ( $lockers{$p4user} )
	  {
		&clearmsg($p4user);
		my $cmd="p4 counter $branch"."_lock_$p4user 0";
		print "Lock released : ",`$cmd`,"\n";
		my @sorted_waiters=(sort { $waiters{$a} <=> $waiters{$b} } keys %waiters);
		if (@sorted_waiters)
		  {
			my $yield_to=$sorted_waiters[0];
			&claim_lockfor( $yield_to );
		  }
	  }
	else
	  {
		print STDERR "You don't have the $branch lock\n";
	  }

  }


sub claim_lockfor
{
  my ($user) = @_;
  $cmd="p4 counter $branch"."_lock_$user 1";
  print "Lock gotten for $user : ",`$cmd`,"\n";
  $cmd="p4 counter $branch"."_wait_$user 0";
  print "Lock gotten for $user : ",`$cmd`,"\n";
  $cmd="p4 counter $branch"."_got_time_$user ".time;
  `$cmd`;
}

sub yield_lock
  {
	&get_status;
	if ( $lockers{$p4user} )
	  {
		# find the highest queued person
		my @sorted_waiters=(sort { $waiters{$a} <=> $waiters{$b} } keys %waiters);
		if (@sorted_waiters)
		  {
			my $yield_to=shift;
			my $first_in_queue=$sorted_waiters[0];
			$yield_to=$first_in_queue if ( (! length( $yield_to )) || ($yield_to eq $p4user ) );
			my $cmd="p4 counter $branch"."_lock_$p4user 0";
			print "Lock released : ",`$cmd`,"\n";
			&claim_lockfor($yield_to);
			my $newtime=$waiters{$first_in_queue}-1;
			$cmd="p4 counter $branch"."_wait_$p4user ".$waiters{$first_in_queue};
			print "queue pos set : ",`$cmd`,"\n";
		  }
	  }
	else
	  {
		print STDERR "You don't have the $branch lock\n";
	  }

  }

sub leave_queue
  {
	&get_status;
	if ( $waiters{$p4user} )
	  {
		&clearmsg($p4user);
		$cmd="p4 counter $branch"."_wait_$p4user 0";
		print "Left Queue : ",`$cmd`,"\n";
	  }
	else
	  {
		print STDERR "You aren't in the queue\n";
	  }

  }

sub get_lock
  {
	&get_status;
	return 0 unless( $lock_free );
	if ( $lockers{$p4user})
	  {
		print "You already have the lock\n";
		return 1;
	  }
	&claim_lockfor( $p4user );
	return 1;
  }


sub queue_lock
  {
	unless( $waiters{$p4user})
	  {
		my $cmd="p4 counter $branch"."_wait_$p4user ".time;
		print `$cmd`;
	  }
  }

sub monitor
  {
#	Win32::Console::Free();
	$last_good_status=time;		# for tracking zombie queue heads
	# gui queue monitor for the lock
	$main_window = Win32::GUI::Window->new(-width => 450, -height => 220,
										   -name => "Main",
										   -text => "$branch P4 lock monitor");
	$main_window->Show();
	$refresh_timer=$main_window->AddTimer('T1', 30000);	# 30 secs is fine if we are not first
	$font = Win32::GUI::Font->new(
								  -size => 14,
								 );

	$lock_status_label=$main_window->AddLabel(-left => 5,
											  -name => "status",
											  -top=>3,
											  -font => $font,
											  -height=>26,
											  -text => "Helloaaaaaaaaaaaaaaaaaaaaa, world",
											  -sunken => 1, frame =>"etched");
	$queue_list=$main_window->AddListbox(
										   -name   => "QueueList",
										   -left   => 5,
										   -top    => 35 );
	$lock_button=$main_window->AddButton( -name =>"Lock",-text=>"Lock");
	$release_button=$main_window->AddButton( -name =>"Release",-text=>"Release");
	$yield_button=$main_window->AddButton( -name =>"Yield",-text=>"Yield");
	$drop_button=$main_window->AddButton( -name =>"Leave",-text=>"Leave Queue");
	$refresh_button=$main_window->AddButton( -name =>"Refresh",-text=>"Refresh");
	$lock_choice = $main_window->AddCombobox( -dropdownlist=>1,
											  -name   => "lockchoice",
											  -text =>"kk",
											  -dropdown=>1,
											  -height=>126,
											  -left   => 100,
											  -top    => 3 );
	$lock_choice->InsertItem("main_src");
	$lock_choice->InsertItem("staging_src");
	$lock_choice->InsertItem("rel_src");
	$lock_choice->Select($lock_choice->FindString(lc($branch)) );

	$gui_setup=1;
	&Main_Resize;
	&get_status;
	&update_window_status;
	Win32::GUI::Dialog()
  }

sub clearmsg
{
  local($name)=@_;
  unlink "$msgdir/$branch"."_$name.msg";
}

sub getmsg
  {
	local($name)=@_;
	undef $msg{$name};
	if (-e "$msgdir/$branch"."_$name.msg")
	{
	  if ( open(MSGFILE,"$msgdir/$branch"."_$name.msg"))
		{
		  $msg{$name}=<MSGFILE>;
		  $msg{$name}=~s/[\r\n]//g;
		  close MSGFILE;
		  print "read message for $name\n";
		}
	}
  }

sub update_window_status
  {
	my $color=[0,128,0];
	my $lock_label="The $branch lock is free";
	$release_button->Disable();
	$lock_button->Enable();
	$drop_button->Disable();
	$yield_button->Disable();
	my $lock_free=1;
	my $have_lock=0;
	my $curcontents;
	my $lockholder;
	my $lockduration;
	my $myqueueposition;
	foreach $_ ( keys(%lockers) )
	{
	  &getmsg($_);
	  $curcontents.=$msg{$_};
	  $lock_free=0;
	  $color=[0,0,0];
	  $lock_label="$_ has had the $branch lock since ".&ptime($hadtime{$_})."\n";
	  $lockholder=$_;
	  $lockduration=time-$hadtime{$_};
	  if( $_ eq $p4user )
		{
		  $lock_label="You have had the $branch lock since ".&ptime($hadtime{$_})."\n";
		  $color = [255,0,0];
		  $release_button->Enable();
		  $lock_button->Disable();
		  $have_lock=1;
		}
	}
	$lock_status_label->Change(-text => " $lock_label", -foreground =>$color);
	my @waitlist;
	$myqueueposition = 0;
	foreach $_ (sort { $waiters{$a} <=> $waiters{$b} } keys(%waiters) )
	  {
	    if ($myqueueposition < 1)
		{
		  $myqueueposition--;
		}
		&getmsg($_);
		push @waitlist,$_;
		if ($_ eq $p4user )
		  {
		    $myqueueposition*=-1;
			$lock_button->Disable();
			$drop_button->Enable();
		  }
		$curcontents.=$_."--$msg{$_}";
	  }

	$yield_button->Enable() if (length($curcontents) && $have_lock);

	my $should_update=($curcontents ne $lastcontents);
	my $head_of_queue;
	$lastcontents = $curcontents;
	if ($should_update)
	  {
		$queue_list->Clear();
		$queue_list->AddString("$lockholder: $msg{$lockholder}") if ($msg{$lockholder});
	  }

	undef %queuename;
	$count=1;
	foreach $_ (sort { $waiters{$a} <=> $waiters{$b} } @waitlist )
	  {
		$head_of_queue=$_ unless length($head_of_queue);
		my $etime=time-$waiters{$_};
		my $line="$count : $_ has been in the queue since ".&ptime($waiters{$_})."";
		if ($should_update)
		  {
			$queuename{$count-1}=$_;
			$queue_list->AddString( $line);
			$queue_list->AddString(" $_: $msg{$_}") if ($msg{$_});
		  }
		$count++;
	  }
	if ( $lock_free && ( $head_of_queue eq $p4user ) )
	  {
		&get_lock;
	  }

#       Update title bar text so that it is imformative when minimized
    my $titletext;
    if ( $lock_free )
	{
	  $titletext = "$branch is free";
	}
	else
	{
	  $hours = int($lockduration / 3600); 
	  $minutes = int($lockduration % 3600 / 60);
	  if ($myqueueposition < 1)
	  {
	    $myqueueposition = "";
	  }
	  else
	  {
	    $myqueueposition = "$myqueueposition";
	  }
	  if ($minutes < 10)
	  {
	    $minutes = "0$minutes";
	  }
	  $titletext = "$branch $lockholder $hours:$minutes $myqueueposition";
	}
	
    $main_window->Change(-text => $titletext);
}

sub lockchoice_Change
{
  $branch=$lock_choice->GetString($lock_choice->SelectedItem());
  &get_status;
  &update_window_status;
}

sub T1_Timer
{
  &get_status;
  &update_window_status;
}

sub Refresh_Click
{
  &get_status;
  &update_window_status;
}

sub Main_Resize
  {
	if ($gui_setup)
	  {
		$lock_status_label->Width( $main_window->Width()-20);
#		$queue_group_box->Width($main_window->Width()-20);
#		$queue_group_box->Height($main_window->Height()-70);
		$queue_list->Width($main_window->Width()-20);
		$queue_list->Height($main_window->Height()-100);
		my $xofs=5;
		my $yofs=$main_window->Height()-60;
		for( \$lock_button, \$release_button, \$yield_button, \$drop_button,\$refresh_button)
		  {
			$$_->Left($xofs);
			$$_->Top($yofs);
			$xofs+=$$_->Width+5;
		  }
		$lock_choice->Top($yofs);
		$lock_choice->Width( 84 );
		$lock_choice->Left( $main_window->Width - 99 );
	  }

  }

sub Lock_Click
  {
	&get_status;
	my $got_it=0;
	if ($lock_free )
	  {
		$got_it=&get_lock;
	  }
	&queue_lock unless ($got_it);
	&get_status;
	&update_window_status;
}

sub Leave_Click
  {
	print "leaving queue";
	&leave_queue;
	&get_status;
	&update_window_status;

  }
sub Release_Click
  {
	&release_lock;
	&get_status;
	&update_window_status;
  }

sub Yield_Click
  {
	my $yieldto="";
	if ( $queue_list->SelectedItem() != -1 )
	  {
		$yieldto = $queuename{ $queue_list->SelectedItem()};
	  }
	&yield_lock($yieldto);
	&get_status;
	&update_window_status;
  }

sub Main_Terminate
{
  return -1;
}

sub ptime24Hour
  {
	local($_)=@_;
	if ($_ > 0 )
	  {
		my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst)=localtime($_);
		return sprintf("%d:%02d:%02d  %d/%d/%d",$hour,$min,$sec,$mon+1,$mday,1900+$year);
	  }
	return "???";
  }

sub ptime
  {
	local($_)=@_;
	if ($_ > 0 )
	  {
		my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst)=localtime($_);

		my ($hour12) = $hour % 12;
		if ($hour12 == 0)
		{
			$hour12 = 12;
		}

		return sprintf("%d:%02d %s %d/%d/%d", $hour12, $min, ($hour > 11) ? "PM" : "AM", $mon+1, $mday, 1900+$year);
	  }
	return "???";
  }
