::Generates Visual Studio 2022 Project using premake5::
@echo off
call ..\Premake\Windows\premake5.exe vs2022 --file=../premake5.lua
IF %ERRORLEVEL% NEQ 0 (PAUSE)