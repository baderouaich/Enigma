## 1.0.0
- Initial Stable Release.

## 1.0.1
- Added library inih (ini file loader)
- Added Config class which loads config ini file
- Now window settings will be loaded from config file in ./Resources/Config/WindowSettings.ini
- Added hardware analytics next to window if enabled in window settings (FPS, RAM & CPU Usage)
- Enabled swap interval to keep FPS 30 by default
- glfwSwapInterval has no effect on Linux https://www.glfw.org/faq.html#33---why-doesnt-glfwswapinterval-work
"Modern graphics drivers have settings that allow users to override an application’s request for (among other things) swap interval. 
If such a setting is enabled, glfwSwapInterval will have no effect." said GLFW.

# 1.1.1
- Added networking libraries curl, cpr (additional zlib, mbedtls for linux builds)
- Removed Unused resources

# 1.2.1
- Added Check for updates functionality in menu bar help -> Check for updates
- Preparing for release v1.2.1 (which has all above features since v1.0.1)


<!-- In Progress
# 1.3.2
- Fixed Clang compilation issue on linux requires pic flag '-fPIC' for libraries (inih, curl, zlib, mbedlts)
- Added CLI option  (--updates | -n)  which checks for updates when using CLI
- Added Database system to save encryption records
- Upgraded imgui library from 1.79 to 1.81 (master branch)
- Upgraded spdlog library from 1.8.0 to 1.8.2
- Upgraded curl library from 7.74.0 to 7.75.0
- Upgraded glfw library from 3.3.0 to 3.3.3
-->