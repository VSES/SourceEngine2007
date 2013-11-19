@echo off
setlocal

if .%1.==.. goto usage

rem ---- Process command line

set to_main_mergeflags=-am

set branchspec=%1
shift

if .%1.==.. goto done_options
set client_flag=-c %1

:next_option
shift
if .%1.==.. goto done_options
if .%1.==.pause. (
	set pause_end=pause
	goto next_option
)

if .%1.==.mutex. (
	set to_main_mergeflags=-at
	goto next_option
)

if .%1.==.from_main. (
	set dir=from_main
	goto next_option
)

if .%1.==.to_main. (
	set dir=to_main
	goto next_option
)

set filespec=%1
goto next_option


rem ---- Done with command line options
:done_options
if not .%dir%.==.. goto dir_set

rem ---- Determine direction of merge from client name
set dir=from_main
p4 %client_flag% info | grep "Client name" | grep -q "main"
if not errorlevel 1 set dir=to_main
p4 %client_flag% info | grep "Client name" | grep -q "personal"
if not errorlevel 1 set dir=from_main

:dir_set
if .%dir%.==.to_main. set reverse_flag=-r
if .%dir%.==.to_main. set from_to=to
if .%dir%.==.to_main. set merge_flags=%to_main_mergeflags%
if .%dir%.==.from_main. set from_to=from
if .%dir%.==.from_main. set merge_flags=-am

echo Integrate %from_to% main, client:
p4 %client_flag% info | grep "Client name" | sed "s/Client name: /    /"
echo Using branchspec:
echo     %branchspec%
if .%filespec%. == .. goto no_filespec
echo Using filespec:
echo     %filespec%
:no_filespec
echo.

echo.
echo Checking if integration needed...
set logfile=%temp%\%random%.log

p4 %client_flag% integrate -1 %reverse_flag% -n -b %branchspec% %filespec% >%logfile% 2>&1

grep -q "ll revision(s) already integrated" %logfile%
if not errorlevel 1 goto nothing_to_do

grep -q "no target file(s) in both client and" %logfile%
if errorlevel 1 goto direction_ok
if .%reverse_flag%.==.. set retry_reverse_flag=-r
set reverse_flag=%retry_reverse_flag%

p4 %client_flag% integrate -1 %reverse_flag% -n -b %branchspec% %filespec% 2>&1 | grep -q "ll revision(s) already integrated"
if not errorlevel 1 goto nothing_to_do

:direction_ok
del /q %logfile%
echo.
echo Starting integration...

p4 %client_flag% opened 2>&1 | grep -q "File(s) not opened on this client"
if not errorlevel 1 goto prep_changelist

echo.
echo WARNING: You have files opened on this client. It is recommended you submit
echo          changes prior to doing an integraton. 
echo.
pause

:prep_changelist

p4 %client_flag% opened 2>&1 | grep -q "edit default"
if errorlevel 1 goto integrate

echo.
echo Moving files in default changelist to new changelist...
p4 %client_flag% change -o | sed -e "s/<enter description here>/Pre-merge default/g" | p4 %client_flag% change -i

:integrate

echo.
echo Syncing...
p4 %client_flag% sync %filespec%

echo.
echo Integrating...

p4 %client_flag% integrate -1 %reverse_flag% -b %branchspec% %filespec%

p4 %client_flag% opened 2>&1 | grep -q "integrate default"
if errorlevel 1 goto nofiles

echo.
echo Creating new changlist, description "Merge %from_to% main (%branchspec%)"
p4 %client_flag% change -o | sed -e "s/<enter description here>/Merge %from_to% main (%branchspec%)/g" | p4 %client_flag% change -i

echo.
echo Resolving...
p4 %client_flag% resolve -n 2>&1 | grep -q "No file(s) to resolve."
if not errorlevel 1 goto end

p4 %client_flag% resolve %merge_flags%
p4 %client_flag% resolve -at .../*.lib
p4 %client_flag% resolve -at .../*.exe
p4 %client_flag% resolve -at .../*.dll
p4 %client_flag% resolve -at .../*.a
p4 %client_flag% resolve -at .../*.pdb

p4 %client_flag% resolve -n %filespec% 2>&1 | grep -q "o file(s) to resolve."
if not errorlevel 1 goto check_baseless

echo.
echo.
echo ** The following files still require resolving:
echo.
p4 %client_flag% resolve -n %filespec% | sed -e "s/ - merging.*//g"
goto check_baseless

:nofiles
Echo no files to integrate?

:check_baseless
p4 %client_flag% integrate -1 %reverse_flag% -n -b %branchspec% %filespec% 2>&1 | grep -q "without -i flag"
if errorlevel 1 goto end
echo.
echo.
echo ************************************************************************
echo ** Some files did not integrate because they require a baseless merge **
echo ** p4mf cannot safely auto-merge -- Manual merge is required,         **
echo ************************************************************************
echo.
echo Break out now to not attempt a baseless integrate 
echo.
pause
p4 %client_flag% integrate -i -1 %reverse_flag% -b %branchspec% %filespec%


goto end

:nothing_to_do
echo All revision(s) already integrated.
goto end

:usage
echo p4mf branch [clientname] [filespec] [from_main to_main]
echo Merge from specified branch
goto end

:end
echo Done.
%pause_end%

