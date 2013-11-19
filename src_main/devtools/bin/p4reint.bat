@echo off
setlocal

echo.
echo WARNING: YOU ARE GOING TO DO A BRUTE FORCE REINTEGRATION
pause
echo.
echo I hope you know what you're doing
pause

if .%1.==.. goto usage
if not .%2.==.. set client_flag=-c %2
if .%2.==.pause. set pause_end=pause
if .%3.==.pause. set pause_end=pause
if .%4.==.pause. set pause_end=pause
set to_main_mergeflags=-at
if .%2.==.mutex. set to_main_mergeflags=-at
if .%3.==.mutex. set to_main_mergeflags=-at
if .%4.==.mutex. set to_main_mergeflags=-at

set dir=from_main
p4 %client_flag% info | grep "Client name" | grep -q "main"
if not errorlevel 1 set dir=to_main

if .%dir%.==.to_main. set reverse_flag=-r
if .%dir%.==.to_main. set from_to=to
if .%dir%.==.to_main. set merge_flags=%to_main_mergeflags%
if .%dir%.==.from_main. set from_to=from
if .%dir%.==.from_main. set merge_flags=-at

echo Integrate %from_to% main, client:
p4 %client_flag% info | grep "Client name" | sed "s/Client name: /    /"
echo Using branchspec:
echo     %1
echo.

echo.
echo Checking if integration needed...

p4 %client_flag% integrate %reverse_flag% -n -f -d -i -b %1 ... 2>&1 | grep -q "All revision(s) already integrated"
if not errorlevel 1 goto nothing_to_do
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
p4 %client_flag% sync

echo.
echo Integrating...

p4 %client_flag% integrate %reverse_flag% -f -d -i -b %1 ... 

p4 %client_flag% opened 2>&1 | grep -q "integrate default"
if errorlevel 1 goto nofiles

echo.
echo Creating new changlist, description "Forced reintegration %from_to% main (%1)"
p4 %client_flag% change -o | sed -e "s/<enter description here>/Forced reintegration %from_to% main (%1)/g" | p4 %client_flag% change -i

echo.
echo Resolving...
p4 %client_flag% resolve -n 2>&1 | grep -q "No file(s) to resolve."
if not errorlevel 1 goto end

p4 %client_flag% resolve %merge_flags%
p4 %client_flag% resolve -at .../*.lib
p4 %client_flag% resolve -at .../*.exe
p4 %client_flag% resolve -at .../*.dll
echo.
echo.
echo Pause carefully examine results before submitting
echo.
echo.
pause

p4 %client_flag% resolve -n 2>&1 | grep -q "No file(s) to resolve."
if not errorlevel 1 goto end

echo.
echo.
echo ** The following files still require resolving:
echo.
p4 %client_flag% resolve -n | sed -e "s/ - merging.*//g"

goto end

:nothing_to_do
echo All revision(s) already integrated.
goto end

:usage
echo p4mf [branch] [clientname]
echo Merge from specified branch
goto end

:nofiles
Echo no files to integrate?

:end
echo Done.
%pause_end%