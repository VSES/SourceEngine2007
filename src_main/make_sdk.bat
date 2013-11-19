for /f "delims=" %%i in (folderlist.txt) do md "sdk1\%%i"
for /f "delims=" %%i in (filelist.txt) do echo F|xcopy "%%i" "sdk1\%%i" /i /z /y
xcopy game sdk1\ /i /z /y /E /EXCLUDE:excludelist.txt
xcopy Game_SDK.sln sdk1\