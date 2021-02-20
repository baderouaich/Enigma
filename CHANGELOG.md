## 1.0.0
- Initial Stable Release.

## 1.0.1
- Added hardware analytics next to window if enabled in window settings (FPS, RAM & CPU Usage)
- Added library inih (ini file loader)
- Added Config class which loads config ini file
- Now window settings will be loaded from config file in ./Resources/Config/WindowSettings.ini
- Enabled swap interval to keep FPS 30 by default
- glfwSwapInterval has no effect on Linux https://www.glfw.org/faq.html#33---why-doesnt-glfwswapinterval-work
"Modern graphics drivers have settings that allow users to override an application’s request for (among other things) swap interval. 
If such a setting is enabled, glfwSwapInterval will have no effect." said GLFW.

# 1.1.1
- Added networking libraries curl, cpr (additional zlib, mbedtls for linux builds)

# 1.2.1
- Added Check for updates functionality in menu bar help -> Check for updates
- Preparing for release v1.2.1 (which has all above features since v1.0.1)
