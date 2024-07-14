# Enigma
[![MIT License](https://img.shields.io/badge/license-MIT-yellow)](https://github.com/baderouaich/Enigma/blob/main/LICENSE.md)
[![Static Analysis Status](https://github.com/baderouaich/Enigma/workflows/static-analysis/badge.svg)](https://github.com/baderouaich/Enigma/actions?workflow=static-analysis)
[![Docs](https://codedocs.xyz/doxygen/doxygen.svg)](https://baderouaich.github.io/Enigma)
[![Language](https://img.shields.io/badge/C++-20-blue.svg?style=flat&logo=c%2B%2B)](https://img.shields.io/badge/C++-20-blue.svg?style=flat&logo=c%2B%2B)


<!--LOGO-->
<!-- ![Enigma](res/branding/EnigmaLogoWolf_860x869.png) -->
<!-- <img src="res/branding/EnigmaLogoWolf_860x869.png" width="330"/> -->
<img src="res/branding/Logo.png" width="250" />

<!--DESCRIOTION-->
A Simple, Reliable and Efficient Encryption Tool <small>(for personal use)</small><br>
<!--
Built to solve 3 problems i had:
- huge personnel files take too much space in storage clouds <strong>(Solved by Compression)</strong>
- if someone accessed my storage's account, you know. <strong>(Solved by Encryption)</strong>
- just can't trust other programs to do the above two for me plus Enigma is a cool name to waste (in memory of Alan Turing)
-->

### CI Status

| Operating system          | Build status                                                                                                                                                                                                            |
|---------------------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Ubuntu GCC (x64)          | [![Ubuntu](https://img.shields.io/github/actions/workflow/status/baderouaich/Enigma/build-ubuntu-gcc.yml?branch=main)](https://github.com/baderouaich/Enigma/actions/workflows/build-ubuntu-gcc.yml)                    |
| Ubuntu GCC Tests (x64)    | [![Ubuntu](https://img.shields.io/github/actions/workflow/status/baderouaich/Enigma/build-ubuntu-gcc-tests.yml?branch=main)](https://github.com/baderouaich/Enigma/actions/workflows/build-ubuntu-gcc-tests.yml)        |
| Ubuntu Clang (x64)        | [![Ubuntu](https://img.shields.io/github/actions/workflow/status/baderouaich/Enigma/build-ubuntu-clang.yml?branch=main)](https://github.com/baderouaich/Enigma/actions/workflows/build-ubuntu-clang.yml)                |
| Ubuntu Clang Tests (x64)  | [![Ubuntu](https://img.shields.io/github/actions/workflow/status/baderouaich/Enigma/build-ubuntu-clang-tests.yml?branch=main)](https://github.com/baderouaich/Enigma/actions/workflows/build-ubuntu-clang-tests.yml)    |
| Windows MinGW (x64)       | [![Windows](https://img.shields.io/github/actions/workflow/status/baderouaich/Enigma/build-windows-mingw.yml?branch=main)](https://github.com/baderouaich/Enigma/actions/workflows/build-windows-mingw.yml)             |
| Windows MinGW Tests (x64) | [![Windows](https://img.shields.io/github/actions/workflow/status/baderouaich/Enigma/build-windows-mingw-tests.yml?branch=main)](https://github.com/baderouaich/Enigma/actions/workflows/build-windows-mingw-tests.yml) |
| Windows MSVC (x64)        | [![Windows](https://img.shields.io/github/actions/workflow/status/baderouaich/Enigma/build-windows-msvc.yml?branch=main)](https://github.com/baderouaich/Enigma/actions/workflows/build-windows-msvc.yml)               |
| Windows MSVC Tests (x64)  | [![Windows](https://img.shields.io/github/actions/workflow/status/baderouaich/Enigma/build-windows-msvc-tests.yml?branch=main)](https://github.com/baderouaich/Enigma/actions/workflows/build-windows-msvc-tests.yml)   |

## Supported Encryption Algorithms
All [Authenticated Encryption](https://www.cryptopp.com/wiki/Authenticated_Encryption)
- :white_check_mark: <strong>AES-GCM</strong> (Recommended)
- :white_check_mark: <strong>Twofish-GCM</strong>
- :white_check_mark: <strong>TripleDES-EAX</strong>
- :white_check_mark: <strong>Blowfish-EAX</strong>
- :white_check_mark: <strong>IDEA-EAX</strong>
- :white_check_mark: <strong>ChaCha20Poly1305</strong>
- :white_check_mark: <strong>RSA-OAEP-SHA256</strong>

## Extra Tools
- Strong Password Generator
- Hashing Algorithms (`MD2`, `MD4`, `MD5`,
`SHA1`, `SHA224`, `SHA256`, `SHA384`, `SHA512`,
`SHAKE128`, `SHAKE256`,
`KECCAK224`, `KECCAK256`, `KECCAK384`, `KECCAK512`,
`RIPEMD128`,`RIPEMD160`,`RIPEMD256`,`RIPEMD320`,
`TIGER`, `WHIRLPOOL`, `SM3`)
- XOR Encryption <small>for nerds</small>
- System & Hardware Information (`CPU`, `RAM`, `GPU`, `OS`, `Kernel`...)

<!--
## Features
- :zap: Very fast Encryption & Decryption
- :hammer_and_wrench: Useful tools like Password Generator, Hashing and System & Hardware Information
- :computer: User Interface & Command-Line Interface support
- :shield: Strongest Encryption Algorithms of the Decade
- :package: GZip Compression by default to reduce cipher size if sharing or uploading files to clouds
- :briefcase: SQLite3 Database system to save, import and export encryption records
- :octocat: Open Source of course!
-->

<!--- :detective: Auto-detect algorithm used for encryption (no need to keep remembering which algorithm you used, just remember your password, and store cipher base64-text or encrypted file somewhere preferably clouds)-->

<!--
## NOTES
- Encryption passwords are NOT saved into the database.
- Import/Export your database by copying or replacing database file `Enigma.db` located in `./res/database/`
-->

## UI
- Main Menu
  ![Main-Menu-Demo](https://github.com/baderouaich/Enigma/assets/49657842/72b69cab-af31-48d1-88b5-86914662c27c)
- Straight forward
  ![Straightforward](https://github.com/baderouaich/Enigma/assets/49657842/52474a80-41bd-4f12-a05b-c2b5579ed34d)
- Supports the strongest encryption algorithms
  ![Supports-Strongest-Encryption-Algorithms](https://github.com/baderouaich/Enigma/assets/49657842/932c260c-3f1c-4333-8267-3e8ae115d408)
- Save Encryptions to a local database (an SQLite3 database which you can export/import)
  ![My-Encryptions-Demo](https://github.com/baderouaich/Enigma/assets/49657842/3e0e7a95-446b-4f48-8bf2-516ab469fdae)
- Extra Utilities
  ![Tools-Demo](https://github.com/baderouaich/Enigma/assets/49657842/064e35ce-2f3f-4f47-9411-e97c2a0b2a25)

### Compiling & Installing
```shell
git clone https://github.com/baderouaich/Enigma
cd Enigma
mkdir build && cd build
cmake .. -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release # or -G"Visual Studio 17 2022" to use the MSVC compiler
make install -j$(nproc) # or -j%NUMBER_OF_PROCESSORS% on Windows
```
- No `sudo` or `Administrator` privileges are needed for installation.
- All Enigma files will be installed to `/home/$(whoami)/Enigma/` (**Linux**) |  `C:\Program Files\Enigma\` (**Windows**)
- To uninstall simply remove the installed `Enigma/` directory. Please make sure you export your encryptions from menu `File -> Export`before removing the directory.

<!--
## Compiling
- <i>Clone Repository</i> : `$ git clone https://github.com/baderouaich/Enigma.git`<br>

### Windows Visual Studio 19 2022
- Extract premake5 binary in `./Premake/Windows/premake-5.0.0-beta1-windows.zip`<br>
- Run `./Scripts/Gen-Windows-vs2019.bat` to generate Visual Studio 16 2019 project solution files
- Open Enigma.sln and Change configuration to Release or Dist
- Build & Run Solution


### Linux GNU Makefile 
> Bellow steps are equivelent to running script `sudo bash ./Scripts/Linux-Build-Release.sh`
 - Install latest c++ compiler (gcc or clang)<br>
  `$ sudo add-apt-repository ppa:ubuntu-toolchain-r/ppa`<br>
  `$ sudo apt update`<br>
  gcc: `$ sudo apt install gcc-12 g++-12` or clang: `$ sudo apt install clang-14 clang++-14`<br>
 - Install necessary libs (opengl, x11, ...)<br>
  `$ sudo apt install libgl1-mesa-dev libxi-dev libx11-dev libxcursor-dev libxrandr-dev libxcomposite-dev libxinerama-dev libtbb-dev`
 - Extract premake5 binary<br>
  `$ tar xvzf ./Premake/Linux/premake-5.0.0-beta1-linux.tar.gz -C ./Premake/Linux/`
 - Run `$ sudo bash ./Scripts/Gen-Linux-Makefile.sh` to generate project Makefiles
 - Compile with gcc: `$ make config=release CC=gcc-12 CPP=g++-12 -j$(nproc)` or clang: `$ make config=release CC=clang-14 CPP=clang++-14 -j$(nproc)` <br> 
 - Run Enigma Binary `$ ./Bin/Release-linux-x86_64/Enigma/Enigma`

### MacOS TODO (when i figure out a way to test macos on vm)
- Run `$ sudo bash ./Scripts/Gen-MacOS-XCode.sh` to generate XCode project files<br> 


 -->

## License
[MIT License](LICENSE.md)


## Features and bugs

If you face any problems feel free to open an issue at the [issue tracker][tracker]. If you feel the program is missing a feature, please raise a ticket on Github. Pull requests are also welcome.

[tracker]: https://github.com/baderouaich/Enigma/issues
