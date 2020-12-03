# Enigma
<!--WORKFLOW-->
![](https://img.shields.io/badge/license-MIT-yellow)
[![Build Status](https://github.com/BaderEddineOuaich/Enigma/workflows/build/badge.svg)](https://github.com/BaderEddineOuaich/Enigma/actions?workflow=build) [![Static Analysis Status](https://github.com/BaderEddineOuaich/Enigma/workflows/static-analysis/badge.svg)](https://github.com/BaderEddineOuaich/Enigma/actions?workflow=static-analysis)

<!--LOGO-->
![Enigma](Resources/Branding/EnigmaLogo_Full.jpg)

<!--DESCRIOTION-->
Encrypt & Decrypt your important data


## Supported Algorithm
- AES-CBC


## Compiling
- <i>Clone Repository</i> : `$ git clone https://github.com/BaderEddineOuaich/Enigma.git` <br>

 <strong>Windows</strong> : <br>
 - Run `Gen-Windows-vs2019.bat` to generate Visual Studio 16 2019 Project<br> 
- Open Enigma.sln and Change configuration to Release or Dist
- Build Solution and Run `./Bin/Release-windows-x86_64/Enigma/Enigma.exe`

<strong>Linux</strong> : <br>
 - Install necessary libs `sudo apt install libx11-dev libxcursor-dev libxrandr-dev libxcomposite-dev libxinerama-dev`
 - Run `Gen-Linux-GMake2.sh` to generate Make Files
 - make config=release (or dist) -j x (replace x with number of threads to use for compilation)
 - Run Enigma binary `./Bin/Release-linux-x86_64/Enigma/Enigma`

<strong>MacOS</strong> : <br>
- Run `Gen-MacOS-XCode.sh` to generate XCode Project<br>
- TODO

## Status (Under Development)
Latest Test on `Windows 10 Version 20H2`
<image src="Resources/Dev Process/Windows-Latest.png"/>
<br>
Latest Test on `Linux Mint Cinnamon 19.3 Tricia`
<image src="Resources/Dev Process/Linux-Latest.png"/>


## License
[MIT License](LICENSE.md)


## Features and bugs

If you face any problems feel free to open an issue at the [issue tracker][tracker]. If you feel the program is missing a feature, please raise a ticket on Github. Pull requests are also welcome.

[tracker]: https://github.com/BaderEddineOuaich/Enigma/issues