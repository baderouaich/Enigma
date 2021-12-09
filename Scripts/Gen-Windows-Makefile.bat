::Generates GMake2 Makefiles using premake5::
@echo off
call ..\Premake\Windows\premake5.exe gmake2  --file=../premake5.lua
IF %ERRORLEVEL% NEQ 0 (PAUSE)