::Generates Visual Studio 2019 Project using premake5.exe and premake5.lua script::
call .\Premake\Windows\premake5.exe vs2019
IF %ERRORLEVEL% NEQ 0 (PAUSE)