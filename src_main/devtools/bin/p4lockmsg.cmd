@echo off

:: // This will make all new env variables local to this script 
setlocal

for /f "tokens=2 delims== " %%A in ('p4 set ^| find /i "P4USER="') do set valveP4User=%%A
if "%valveP4User%"=="" (
	echo Error calling command! Perforce username variable not defined!
	endlocal
	exit /b 1
)

if .%1.==.. (
	echo.
	echo  Usage: p4lockmsg This is my message.
	echo.
	echo  No message given. Deleting \\fileserver\user\perforce\main_src_%valveP4User%.msg
	if EXIST \\fileserver\user\perforce\main_src_%valveP4User%.msg del /q \\fileserver\user\perforce\main_src_%valveP4User%.msg
	endlocal
	exit /b 0
)

echo %* > \\fileserver\user\perforce\main_src_%valveP4User%.msg

endlocal
exit /b 0
