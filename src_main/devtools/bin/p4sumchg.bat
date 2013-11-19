@echo off
if .%_4ver%.==.. goto need4dos
setlocal

if .%1.==.. goto nouser
set user=%1

p4 users | grep -i -s -q -w %1
if errorlevel==1 echoerr %user% is not an active user

rem -----------------------------------
iff .%2.==.. then
	set fromdate=%@date[1990-1-1]
else
	set fromdate=%@date[%@replace[/,-,%2]]
endiff

echoerr Summing changes for %user since %@makedate[%fromdate]

rem -----------------------------------
set changes=%@unique[.]
p4 changes -u %user% -s submitted > %changes%

set changelists=%@unique[.]

echoserr Gathering changelists...
set numchangelists=0
do a in @%changes%
	echoserr ...
	if %@date[%@replace[/,-,%@word[3,%a]]] LT %fromdate% leave
	echo %@word[1,%a] >> %changelists%
	set numchangelists=%@inc[%numchangelists]
	echoserr ^b^b^b`   `^b^b^b
enddo

set srcexts=.cpp .h

set diffs=%@unique[%temp%]
set addcount=%@unique[%temp%]

set addedfiles=0
set addedfileslines=0

echoerr.
set remaining=%numchangelists
do curchangelist in @%changelists%
	p4 describe -s %curchangelist | grep "... /" | grep -i -w -E "[0-9]+ ((edit)|(add))$" > %changes%
	if errorlevel==1 set numchangelists=%@dec[%numchangelists]
	set remaining=%@dec[%remaining]
	echoserr Gathering diffs for changelist %curchangelist (%remaining remaining)...`    `
	do a in @%changes%
		set curfile=%@word[" #",1,%a]
		iff .%@ext[%curfile]. != .. .AND. %@index[%srcexts,%@lower[.%@ext[%curfile]]] != -1 then
			iff %@word[-0,%a] == edit then
				p4 diff2 -dsbw %curfile%@%@eval[%curchangelist%-1] %curfile%@%curchangelist% >> %diffs%
			elseiff %@word[-0,%a] == add then
				set addedfiles=%@inc[%addedfiles]
				p4 print %curfile%@%curchangelist% > %addcount%
				set addedfileslines=%@eval[ %addedfileslines + %@lines[%addcount%] ]
			endiff
		endiff
	enddo
	echoserr ^r
enddo

set files=%addedfiles
set addchunks=0
set addlines=0
set deletechunks=0
set deletelines=0
set changechunks=0
set changelinesfrom=0
set changelinesto=0

echoerr.
echoserr Summarizing diffs...
do a in @%diffs%
	echoserr ...
	set type=%@word[0,%a]
	iff .%type.==.add. then
		set addchunks=%@eval[%addchunks% + %@word[1,%a] ]
		set addlines=%@eval[%addlines% + %@word[3,%a] ]
	elseiff .%type.==.deleted. then
		set deletechunks=%@eval[%deletechunks% + %@word[1,%a] ]
		set deletelines=%@eval[%deletelines% + %@word[3,%a] ]
	elseiff .%type.==.changed. then
		set changechunks=%@eval[%changechunks% + %@word[1,%a] ]
		set changelinesfrom=%@eval[%changelinesfrom% + %@word[3,%a] ]
		set changelinesto=%@eval[%changelinesto% + %@word[5,%a] ]
	else
		set files=%@inc[%files]
	endiff
	echoserr ^b^b^b`   `^b^b^b
enddo

echo.
echo User %user changed %files files in %numchangelists changelists: add %addchunks chunks %addlines lines, deleted %deletechunks chunks %deletelines lines, changed %changechunks chunks %changelinesfrom / %changelinesto lines, %addedfiles files added for %addedfileslines lines

goto end

rem ---------------------------------------------------------------------------
:need4dos
echo This batch file requires 4nt
goto end

rem ---------------------------------------------------------------------------
:nouser
echo No user specified.
goto end

rem ---------------------------------------------------------------------------
:invaliduser
echo Not a valid user.
goto end

rem ---------------------------------------------------------------------------
:end
del /q/e %changes%
del /q/e %changelists%
del /q/e %diffs%
del /q/e %addcount%
