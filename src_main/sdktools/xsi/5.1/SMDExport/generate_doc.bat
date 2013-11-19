echo off
echo Cleaning...
del /Q C:\Work\SMD\SMDExport\doc\*.*

echo Generating Doxygen Valve Tools Documentation
v:\storm\doxygen\bin\doxygen doxygen_valve

echo Compiling .chm
copy ValveTools.hhp .\doc\html
cd .\doc\html
"c:\Program Files\HTML Help Workshop\hhc.exe" ValveTools.hhp