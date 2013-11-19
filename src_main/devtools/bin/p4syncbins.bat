@echo off
setlocal
if not .%1. == .. set ver=%1
if .%1. == .. set ver=head
echo Searching for inconsistent files...
echo.
if exist %temp%\p4syncbins.tmp del /q %temp%\p4syncbins.tmp
p4 diff -f -ds -se -sa -sd .\...exe#%ver% > %temp%\p4syncbins.tmp 2> nul
p4 diff -f -ds -se -sa -sd .\...dll#%ver% >> %temp%\p4syncbins.tmp 2> nul
p4 diff -f -ds -se -sa -sd .\...pdb#%ver% >> %temp%\p4syncbins.tmp 2> nul
p4 diff -f -ds -se -sa -sd .\...map#%ver% >> %temp%\p4syncbins.tmp 2> nul
p4 diff -f -ds -se -sa -sd .\...lib#%ver% >> %temp%\p4syncbins.tmp 2> nul
echo Will force sync of the following files to "%ver%":
type %temp%\p4syncbins.tmp | sort
echo.
pause
for /f "delims=" %%a in (%temp%\p4syncbins.tmp) do p4 sync -f %%a#%ver%
