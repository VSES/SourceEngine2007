@echo off
setlocal
set ver=have
echo Searching for inconsistent files...
echo.
if exist %temp%\p4syncbins.tmp del /q %temp%\p4syncbins.tmp
p4 diff -f -ds -se -sa .\...exe#%ver% > %temp%\p4syncbins.tmp 2> nul
p4 diff -f -ds -se -sa .\...dll#%ver% >> %temp%\p4syncbins.tmp 2> nul
p4 diff -f -ds -se -sa .\...pdb#%ver% >> %temp%\p4syncbins.tmp 2> nul
p4 diff -f -ds -se -sa .\...map#%ver% >> %temp%\p4syncbins.tmp 2> nul
p4 diff -f -ds -se -sa .\...lib#%ver% >> %temp%\p4syncbins.tmp 2> nul
echo Will open the following files for edit:
type %temp%\p4syncbins.tmp | sort
echo.
pause
for /f "delims=" %%a in (%temp%\p4syncbins.tmp) do p4 edit %%a
