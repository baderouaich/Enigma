::Generates Visual Studio 2019 Project using premake5::
@echo off
call ..\Premake\Windows\premake5.exe vs2019 --file=../premake5.lua
IF %ERRORLEVEL% NEQ 0 (PAUSE)