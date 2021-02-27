@echo off
echo Cleaning Up ...

echo Deleting Bin/ and Bin-Intermediate/ ...
FOR /d /r . %%d IN (Bin) DO @IF EXIST "%%d" rd /s /q "%%d"
FOR /d /r . %%d IN (Bin-Intermediate) DO @IF EXIST "%%d" rd /s /q "%%d"

echo Deleting *.sln *.vcxproj *.vcxproj.filters ...
del /s /q /f *.sln
del /s /q /f *.vcxproj.*

echo Done.
PAUSE