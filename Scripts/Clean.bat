@echo off
echo Cleaning Up ...

echo Going back to projects directory..
pushd %~dp0\..\

echo Deleting Bin/ and Bin-Intermediate/ ...
FOR /d /r . %%d IN (Bin) DO @IF EXIST "%%d" rd /s /q "%%d"
FOR /d /r . %%d IN (Bin-Intermediate) DO @IF EXIST "%%d" rd /s /q "%%d"

echo Deleting *.sln *.vcxproj *.vcxproj.filters ...
del /s /q /f *.sln
del /s /q /f *.vcxproj.*
del /s /q /f Makefile
del /s /q /f *.make
cd Scripts

echo Done.