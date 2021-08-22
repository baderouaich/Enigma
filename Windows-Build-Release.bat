@echo off

echo Cleaning up for a fresh build...
call ./Clean.bat


:: unzip -h for more
echo Extracting premake5 binary using unzip ... 
unzip -l -o "./Premake/Windows/premake-5.0.0-alpha16-windows.zip" -d "./Premake/Windows/" 


echo Generating Visual Studio 16 2019 project solution files ...
call ./Gen-Windows-vs2019.bat


echo Opening Enigma.sln in Visual Studio ...
start ./Enigma.sln


echo ################# One More Last Step #################
echo Change Configuration to Release mode and Build solution 
echo ######################################################
echo DONE!

PAUSE