# Enigma
<!--WORKFLOW-->
[![MIT License](https://img.shields.io/badge/license-MIT-yellow)](https://github.com/BaderEddineOuaich/Enigma/blob/master/LICENSE.md)
[![Build Status](https://github.com/BaderEddineOuaich/Enigma/workflows/build/badge.svg)](https://github.com/BaderEddineOuaich/Enigma/actions?workflow=build) 
[![Static Analysis Status](https://github.com/BaderEddineOuaich/Enigma/workflows/static-analysis/badge.svg)](https://github.com/BaderEddineOuaich/Enigma/actions?workflow=static-analysis)


<!--LOGO-->
<!-- ![Enigma](Resources/Branding/EnigmaLogoWolf_860x869.png) -->
<!-- <img src="Resources/Branding/EnigmaLogoWolf_860x869.png" width="330"/> -->
<img src="Resources/Branding/Wolf.png" width="333"/>

<!--DESCRIOTION-->
A Simple, Reliable and Efficient UI + CLI Encryption Tool


## Supported Algorithms
- :white_check_mark: <strong>AES-GCM</strong> (Recommended)
- :white_check_mark: <strong>ChaCha20</strong>
- :white_check_mark: <strong>TripleDES-CBC</strong>
- :white_check_mark: <strong>Twofish-GCM</strong>
- :white_check_mark: <strong>IDEA-CBC</strong>
<!-- - [ ] algo || [x] algo -->


## Features
- :zap: Very fast encryption/decryption
- :computer: User Interface & Command-Line Interface support
- :shield: Strongest encryption algorithms of the decade
- :detective: Auto-detect algorithm used for encryption (no need to keep remembering which algorithm you used, just remember your password, and store cipher base64-text or encrypted file somewhere preferably clouds)
- :package:	GZip compression to reduce size of files if sharing or uploading to clouds
- :page_facing_up: Open Source ofcourse!


## Enigma CLI
```text
Usage:
  Enigma [OPTION...]

  -e, --encrypt                 Encrypt operation
  -d, --decrypt                 Decrypt operation
  -m, --mode [AES, ChaCha20, TripleDES, Twofish, IDEA]
                                Encryption/Decryption mode (default: AES)
  -p, --password arg            Encryption password
  -t, --text arg                Text to Encrypt/Decrypt
  -i, --infile arg              Input File to Encrypt/Decrypt
  -o, --outfile arg             Output File to Encrypt/Decrypt
  -c, --compress                Compress File Before Encrypting
  -u, --decompress              Decompress File After Decrypting
  -h, --help                    Displays help message
  -v, --version                 Displays Enigma's version
```
- Example encypting a text with AES-GCM:<br>
```text
./Enigma -e --mode=aes --password="My@pa$$W0rd" --text="my important data securly encrypted!"
output: AaxsjxUSbOmTIjmT90WrYlqg7I7dFY7e+OqPaX20DusVnBLpkg4LW0f/fWpKES8gzFb1SrglpVt0goGfijzVHx+ULjuT
```
- Example decrypting a text with AES-GCM:<br>
```text
./Enigma -d --password="My@pa$$W0rd" --text="AaxsjxUSbOmTIjmT90WrYlqg7I7dFY7e+OqPaX20DusVnBLpkg4LW0f/fWpKES8gzFb1SrglpVt0goGfijzVHx+ULjuT"
output: my important data securly encrypted!
```
<image src="Resources/ScreenShots/CLI-EncryptDecryptText-Demo.png"/><br>

- Example encypting a file with AES-GCM:<br>
```text
./Enigma -e --mode=aes --password="My@pa$$W0rd" --infile="/home/bader/Desktop/Data" --outfile="/home/bader/Desktop/Data-encrypted.enigma" --compress
```
- Example decypting a file with AES-GCM:<br>
```text
./Enigma -d --password="My@pa$$W0rd" --infile="/home/bader/Desktop/Data-encrypted.enigma" --outfile="/home/bader/Desktop/Data-decrypted" --decompress
```
<image src="Resources/ScreenShots/CLI-EncryptDecryptFile-Demo.png"/><br>



## Enigma UI
<image width="300" src="Resources/ScreenShots/UI-MainMenuScene.png"/>  <image width="300" src="Resources/ScreenShots/UI-EncryptText-Demo.png"/>  <image width="300" src="Resources/ScreenShots/UI-DecryptText-Demo.png"/>  <image width="300" src="Resources/ScreenShots/UI-EncryptFile-Demo.png"/>  <image width="300" src="Resources/ScreenShots/UI-DecryptFile-Demo.png"/>
<!--
Latest Test on `Windows 10 Version 20H2`
<image src="Resources/Dev Process/Windows-Latest.png"/>
<br>
Latest Test on `Linux Mint Cinnamon 19.3 Tricia`
<image src="Resources/Dev Process/Linux-Latest.png"/>
-->


## Compiling
- <i>Clone Repository</i> : `$ git clone https://github.com/BaderEddineOuaich/Enigma.git`<br>

### Windows
 - Extract premake5 binary in `./Premake/Windows/premake-5.0.0-alpha15-windows.zip`<br><br>
<strong>Method 1: Visual Studio 16 2019</strong>
 - Run `Gen-Windows-vs2019.bat` to generate Visual Studio 16 2019 Project<br> 
 - Open Enigma.sln and Change configuration to Release or Dist
 - Build Solution<br>
 - Copy Resources/ folder next to the executable at `.\Bin\Release-windows-x86_64\Enigma\Enigma.exe` and Run it<br><br>
<strong>Method 2 (Unstable): Makefile (GMake2)</strong>
 - Run `Gen-Windows-Makefile.bat` to generate project Makefiles<br>
 - Run `make config=release CC="path/to/your/c/compiler" CXX="path/to/your/c++/compiler"`<br>
   example: `make config=release CC=D:/MinGW/10.1.0/mingw64/bin/gcc.exe  CXX=D:/MinGW/10.1.0/mingw64/bin/g++.exe -j 2`<br>
 - Copy `Resources/` folder next to the executable at `.\Bin\Release-windows-x86_64\Enigma\Enigma.exe` and Run it

### Linux
 - Install Latest gcc or clang Compiler<br>
  `sudo add-apt-repository ppa:ubuntu-toolchain-r/ppa`<br>
	`sudo apt update`<br>
	gcc `sudo apt install gcc-10 g++-10` or clang `sudo apt install clang-10 clang++-10`<br>
 - Install necessary libs<br>
  `sudo apt install libgl1-mesa-dev libxi-dev libx11-dev libxcursor-dev libxrandr-dev libxcomposite-dev libxinerama-dev`
 - Extract premake5 binary <br>
  `tar xvzf ./Premake/Linux/premake-5.0.0-alpha15-linux.tar.gz -C ./Premake/Linux/`
 - Run `sudo bash ./Gen-Linux-Makefile.sh` to generate project Makefiles
 - Compile with gcc `make config=release CC=gcc-10 CPP=g++-10 -j X` or clang `make config=release CC=clang-10 CPP=clang++-10 -j X` <br> 
   replace X with number of threads to use for compilation, the more the faster<br>
   replace release with dist for full performance
 - Copy `Resources/` folder next to the executable at `./Bin/Release-linux-x86_64/Enigma/Enigma`
 - Run Enigma Binary `./Bin/Release-linux-x86_64/Enigma/Enigma`

### MacOS TODO (when i figure out a way to test macos on vm)
- Run `sudo bash ./Gen-MacOS-XCode.sh` to generate XCode Project<br>


## License
[MIT License](LICENSE.md)


## Features and bugs

If you face any problems feel free to open an issue at the [issue tracker][tracker]. If you feel the program is missing a feature, please raise a ticket on Github. Pull requests are also welcome.

[tracker]: https://github.com/BaderEddineOuaich/Enigma/issues
