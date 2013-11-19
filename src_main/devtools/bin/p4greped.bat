@echo off

if .%1.==.. goto end

grep -q %1 *.cpp *.h
if errorlevel 1 goto end

grep -l %1 *.cpp *.h | p4 -x- edit 

:end
