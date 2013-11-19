@echo off
p4 changes -s submitted @%1,@now > %temp%\p4changeddirs_changes.lst
if exist %temp%\p4changeddirs_changedescs.lst del %temp%\p4changeddirs_changedescs.lst
for /F "tokens=2" %%a in (%temp%\p4changeddirs_changes.lst) do p4 describe -s %%a >> %temp%\p4changeddirs_changedescs.lst
grep "... //" %temp%\p4changeddirs_changedescs.lst | sed "s/... //" | sed "s/#.*//" | sort | uniqexe
