# Enigma
<!--WORKFLOW-->
[![MIT License](https://img.shields.io/badge/license-MIT-yellow)](https://github.com/BaderEddineOuaich/Enigma/blob/master/LICENSE.md)
[![Build Status](https://github.com/BaderEddineOuaich/Enigma/workflows/build/badge.svg)](https://github.com/BaderEddineOuaich/Enigma/actions?workflow=build) 
[![Static Analysis Status](https://github.com/BaderEddineOuaich/Enigma/workflows/static-analysis/badge.svg)](https://github.com/BaderEddineOuaich/Enigma/actions?workflow=static-analysis)
[![Docs](https://codedocs.xyz/doxygen/doxygen.svg)](https://badereddineouaich.github.io/Enigma)


<!--LOGO-->
<!-- ![Enigma](Resources/Branding/EnigmaLogoWolf_860x869.png) -->
<!-- <img src="Resources/Branding/EnigmaLogoWolf_860x869.png" width="330"/> -->
<img src="Resources/Branding/Logo.png" width="333" />

<!--DESCRIOTION-->
A Simple, Reliable and Efficient UI + CLI Encryption Tool<br>
<!--
Built to solve 3 problems i had:
- huge personal files take too much space in storage clouds <strong>(Solved by Compression)</strong>
- if someone accessed my storage's account, you know. <strong>(Solved by Encryption)</strong>
- just can't trust other programs to do the above two for me plus Enigma and is a cool name to waste (in memory of Alan Turing)
-->



## Supported Algorithms
All [Authenticated Encryption](https://www.cryptopp.com/wiki/Authenticated_Encryption)
- :white_check_mark: <strong>AES-GCM</strong> (Recommended)
- :white_check_mark: <strong>Twofish-GCM</strong>
- :white_check_mark: <strong>TripleDES-EAX</strong>
- :white_check_mark: <strong>Blowfish-EAX</strong>
- :white_check_mark: <strong>IDEA-EAX</strong>
- :white_check_mark: <strong>ChaCha20Poly1305</strong>
<!-- - :white_check_mark: <strong>RSA-OAEP-SHA256</strong> -->
<!-- - [ ] algo || [x] algo -->


## Available Tools
- Password Generator
- Hashing Algorithms (MD2, MD4, MD5, SHA1, SHA224, SHA256, SHA384, SHA512, SHAKE128, SHAKE256, KECCAK224, KECCAK256, KECCAK384, KECCAK512, TIGER, WHIRLPOOL)
- XOR Encryption
- System & Hardware Information

<!--
## Features
- :zap: Very fast Encryption & Decryption
- :hammer_and_wrench: Useful tools like Password Generator, Hashing and System & Hardware Information
- :computer: User Interface & Command-Line Interface support
- :shield: Strongest Encryption Algorithms of the Decade
- :package: GZip Compression by default to reduce cipher size if sharing or uploading to clouds
- :briefcase: SQLite3 Database system to save, import and export encryption records
- :octocat: Open Source of course!
-->

<!--- :detective: Auto-detect algorithm used for encryption (no need to keep remembering which algorithm you used, just remember your password, and store cipher base64-text or encrypted file somewhere preferably clouds)-->

<!--
## NOTES
- Encryption passwords are NOT saved into the database.
- Import/Export your database by copying or replacing database file `Enigma.db` located in `./Resources/Database/`
-->

## Enigma CLI
```text
Usage:
  Enigma [OPTION...]

  -e, --encrypt                 Encrypt operation
  -d, --decrypt                 Decrypt operation
  -a, --algorithm [AES, Twofish, TripleDES, Blowfish, IDEA, ChaCha20] Encryption/Decryption algorithm (default: AES)
  -p, --password arg            Encryption password
  -t, --text arg                Text to Encrypt or Decrypt (base64)
  -i, --infile arg              Input File to Encrypt/Decrypt
  -o, --outfile arg             Output File to Encrypt/Decrypt
  -s, --save                    Save encryption record to database
  -l, --list                    List saved encryption records from database
  -h, --help                    Displays help message
  -v, --version                 Displays Enigma's version
      --check-for-updates       Check for new version releases
```

- Example encypting a text with AES-GCM:<br>
```batch
./Enigma -e --algorithm=aes --password="My@pa$$W0rd" --text="my important data securly encrypted!"
output: AaxsjxUSbOmTIjmT90WrYlqg7I7dFY7e+OqPaX20DusVnBLpkg4LW0f/fWpKES8gzFb1SrglpVt0goGfijzVHx+ULjuT
```
- Example decrypting a text with AES-GCM:<br>
```batch
./Enigma -d --password="My@pa$$W0rd" --text="AaxsjxUSbOmTIjmT90WrYlqg7I7dFY7e+OqPaX20DusVnBLpkg4LW0f/fWpKES8gzFb1SrglpVt0goGfijzVHx+ULjuT"
output: my important data securly encrypted!
```
<!--<img src="Resources/ScreenShots/CLI-EncryptDecryptText-Demo.png"/><br>-->

- Example encypting a file with AES-GCM:<br>
```batch
./Enigma -e --algorithm=aes --password="My@pa$$W0rd" --infile="/home/user/Desktop/Data.txt" --outfile="/home/user/Desktop/Data_encrypted.txt.enigma"
```
- Example decypting a file with AES-GCM:<br>
```batch
./Enigma -d --password="My@pa$$W0rd" --infile="/home/user/Desktop/Data_encrypted.txt.enigma" --outfile="/home/user/Desktop/Data_decrypted.txt"
```
<!-- <img src="Resources/ScreenShots/CLI-EncryptDecryptFile-Demo.png"/><br> -->
<img src="Resources/ScreenShots/4.png"/><br>



## Enigma UI
<img src="Resources/ScreenShots/Main-Menu-Demo.png"/>
<img src="Resources/ScreenShots/2.png"/>
<img src="Resources/ScreenShots/My-Encryptions-Demo.png"/>
<img src="Resources/ScreenShots/Tools-Demo.png"/>

<!--
<img width="300" src="Resources/ScreenShots/UI-MainMenuScene.png"/>  <img width="300" src="Resources/ScreenShots/UI-EncryptText-Demo.png"/>  <img width="300" src="Resources/ScreenShots/UI-DecryptText-Demo.png"/>  <img width="300" src="Resources/ScreenShots/UI-EncryptFile-Demo.png"/>  <img width="300" src="Resources/ScreenShots/UI-DecryptFile-Demo.png"/>
-->

<!--
Latest Test on `Windows 10 Version 20H2`
<img src="Resources/Dev Process/Windows-Latest.png"/>
<br>
Latest Test on `Linux Mint Cinnamon 19.3 Tricia`
<img src="Resources/Dev Process/Linux-Latest.png"/>
-->


## Compiling
- <i>Clone Repository</i> : `$ git clone https://github.com/BaderEddineOuaich/Enigma.git`<br>

### Windows Visual Studio 16 2019 
> Bellow steps are equivelent to running script `./Windows-Build-Release.bat`
 - Extract premake5 binary in `./Premake/Windows/premake-5.0.0-alpha16-windows.zip`<br>
 - Run `Gen-Windows-vs2019.bat` to generate Visual Studio 16 2019 project solution files
 - Open Enigma.sln and Change configuration to Release or Dist
 - Build & Run Solution

<!--
### Windows (equivelent to running `./Windows-Build-Release.bat`)
 - Extract premake5 binary in `./Premake/Windows/premake-5.0.0-alpha16-windows.zip`<br><br>
<strong>Method 1 (Recommended): Visual Studio 16 2019</strong>
 - Run `Gen-Windows-vs2019.bat` to generate Visual Studio 16 2019 project solution files
 - Open Enigma.sln and Change configuration to Release or Dist
 - Build & Run Solution
<strong>Method 2 (Unstable): Makefile (GMake2)</strong>
 - Run `Gen-Windows-Makefile.bat` to generate project Makefiles<br>
 - Run `$ make config=release CC="path/to/your/c/compiler/gcc.exe" CXX="path/to/your/c++/compiler/g++.exe"`<br>
   example: `$ make config=release CC=D:/MinGW/10.1.0/mingw64/bin/gcc.exe CXX=D:/MinGW/10.1.0/mingw64/bin/g++.exe -j 4`<br>
-->
### Linux GNU Makefile 
> Bellow steps are equivelent to running script `sudo bash ./Linux-Build-Release.sh`
 - Install latest c++ compiler (gcc or clang)<br>
  `$ sudo add-apt-repository ppa:ubuntu-toolchain-r/ppa`<br>
  `$ sudo apt update`<br>
  gcc: `$ sudo apt install gcc-10 g++-10` or clang: `$ sudo apt install clang-10 clang++-10`<br>
 - Install necessary libs (opengl, x11, ...)<br>
  `$ sudo apt install libgl1-mesa-dev libxi-dev libx11-dev libxcursor-dev libxrandr-dev libxcomposite-dev libxinerama-dev libtbb-dev`
 - Extract premake5 binary<br>
  `$ tar xvzf ./Premake/Linux/premake-5.0.0-alpha16-linux.tar.gz -C ./Premake/Linux/`
 - Run `$ sudo bash ./Gen-Linux-Makefile.sh` to generate project Makefiles
 - Compile with gcc: `$ make config=release CC=gcc-10 CPP=g++-10 -j 4` or clang: `$ make config=release CC=clang-10 CPP=clang++-10 -j 4` <br> 
<!--  replace -j X with number of threads to use for compilation, the more the faster<br>
   replace release with dist for full performance -->
<!--  - Copy `Resources/` folder next to the executable at `./Bin/Release-linux-x86_64/Enigma/Enigma` --> 
 - Run Enigma Binary `$ ./Bin/Release-linux-x86_64/Enigma/Enigma`

### MacOS TODO (when i figure out a way to test macos on vm)
- Run `$ sudo bash ./Gen-MacOS-XCode.sh` to generate XCode project files<br>


## License
[MIT License](LICENSE.md)


## Features and bugs

If you face any problems feel free to open an issue at the [issue tracker][tracker]. If you feel the program is missing a feature, please raise a ticket on Github. Pull requests are also welcome.

[tracker]: https://github.com/BaderEddineOuaich/Enigma/issues
