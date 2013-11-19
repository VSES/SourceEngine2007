echo off
rem =======================================
rem Install AxisInterpOp_cpp Example 
rem
rem Please run from a XSI Command Prompt.
rem 
rem XSI_USERHOME : points to the user home 
rem directory
rem =======================================

set MODULE=AxisInterpOp

if "%XSI_USERHOME%"=="" goto error

set BINDIR=%XSI_USERHOME%\Application\bin\nt-x86

if not exist "%BINDIR%" mkdir %BINDIR%

xcopy Debug\%MODULE%D.dll %MODULE%.dll
xsibatch -i %MODULE%.spdl -overwrite

goto end
:error
echo Please run from XSI Command Prompt.
:end
