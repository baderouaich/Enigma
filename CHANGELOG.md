# 3.2.0
- New Encryption Algorithm `Camelia-256-GCM`
- New Encryption Algorithm `Serpent-256-GCM`
- New Hashing Algorithms in `Tools -> Hashing` utility:
  `SM3`, `RIPEMD128`, `RIPEMD160`, `RIPEMD256`, `RIPEMD256`
- Multiple input source `Tools -> Hashing`. Now you can compute the hash of a file alongside text
- UI view updates (fonts, colors)
- Upgraded ImGui from v1.90.8 to v1.90.9

# 3.1.0
- RSA Algorithm support for text and large files
- Display algorithm used in encryption in My Encryptions Scene

# 3.0.0
- Complete build system migration from Premake to CMake.
- Large files support. Now you can encrypt large files in chunks with less memory usage.
- Install Enigma user-wide. Now you can `make install` Enigma user wide. All Enigma files will be installed to `/home/$(whoami)/Enigma/` (**Linux**) |  `C:\Program Files\Enigma\` (**Windows**)
- Export & Import your saved encryptions. Now you can use menu items in `File -> Export/Import` to export or import your SQLite3 database file which contains your saved encryption ciphers.
- CLI is no longer supported as I noticed the main use is UI, maybe it will be back
in the upcoming versions if requested.
- Fixed some bugs with MessageDialog displaying only a portion of message text.
- Upgraded ImGui from v1.83 to v1.90.8
- Upgraded Crypto++ from v8.4 to v8.9.0
- Upgraded GLFW from v3.3.0 to v3.4

# 2.2.0
- Fixed wrong text displayed in 'My Encryptions Scene' when searching for nonexistent encryption, it displays "No encryption saved yet" instead of "No encryption found".

# 2.1.0
- Reduced program CPU usage by adding `GLFW 3+ glfwWaitEvents()` in the main loop to wait for events. 
- Remembering encrypted file extension for decryption.
- Fixed some typos in logs and message boxes.
- Handle abnormal exists to normally end program and release resources gracefully.
- Added keyboard shortcuts to main menu scene.
- Fixed ChaCha20Poly1305 bug 
- Upgraded ImGui from v1.83 to v1.85
- Upgraded GLFW from v3.3.0 to v3.3.6
- Upgraded Crypto++ from v8.4 to v8.6

# 2.0.0
- Changed CLI option --mode,-m to --algorithm,-a
- Fix check for updates bug
- Proceeded Doxygen Documentation (~0.60% Documented)


# 1.4.0
- Upgraded portable-file-dialogs library after issue https://github.com/samhocevar/portable-file-dialogs/issues/50 Fixed
- Upgraded ImGui library from 1.81 to 1.83 (master branch)
- Now file paths are runtime detected (which solves the issue of calling CLI from an external folder doesn't work properly)
- Added ability to drop files in window to Encrypt or Decrypt
- Added Password Generator Tool
- Added Hashing Tool
- Added System & Hardware Information Tool
- Started Doxygen Documentation (~0.25% Documented)


# 1.3.0
- Fixed Clang compilation issue on Linux requires pic flag '-fPIC' for libraries (inih, curl, zlib, mbedlts)
- Added CLI option `--check-for-updates` which checks for updates when using CLI
- Upgraded imgui library from 1.79 to 1.81 (master branch)
- Upgraded spdlog library from 1.8.0 to 1.8.2
- Upgraded curl library from 7.74.0 to 7.75.0
- Upgraded glfw library from 3.3.0 to 3.3.3
- Added SQLite3 Database system to save, import and export encryption records
- Now heavy work is being done in background (parallel)
- Now Back button for each scene is placed in the top left instead of bottom to save space
- New CLI Option `--list` or `-l` which lists all saved encryption records in a table
- Ability to decrypt encryption record from database
- Upgraded premake5 from 5.0.0-alpha15 to 5.0.0-alpha16
- Switched Algorithms from CBC padding to Authentication encryption EAX (TripleDES-CBC to TripleDES-EAX, IDEA-CBC to IDEA-EAX, ChaCha20 to ChaCha20Poly1305)
- Added Blowfish-EAX Algorithm
- Changed logging format
- Now buffer is always compressed by default before encrypting
- Now algorithm is auto-detected by default when decrypting, which removes the "detect algorithm used for encryption button"



# 1.2.1
- Added Check for updates functionality in menu bar help -> Check for updates
- Preparing for release v1.2.1 (which has all above features since v1.0.1)



# 1.1.0
- Added networking libraries curl, cpr (additional zlib, mbedtls for linux builds)
- Removed Unused resources



# 1.0.1
- Added library inih (ini file loader)
- Added Config class which loads config ini file
- Now window settings will be loaded from config file in ./Resources/Config/WindowSettings.ini
- Added hardware analytics next to window if enabled in window settings (FPS, RAM & CPU Usage)
- Enabled swap interval to keep FPS 30 by default
- glfwSwapInterval has no effect on Linux https://www.glfw.org/faq.html#33---why-doesnt-glfwswapinterval-work
"Modern graphics drivers have settings that allow users to override an application�s request for (among other things) swap interval. 
If such a setting is enabled, glfwSwapInterval will have no effect." said GLFW.



# 1.0.0
- Initial Stable Release.
