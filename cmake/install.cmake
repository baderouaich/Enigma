#########################
# Installation Structures
#########################
## Linux:
# /home/$(whoami)/Enigma/Enigma                             (executable)
# /home/$(whoami)/Enigma/res/                               (resources: database, config, logs, icons, ...)
# /home/$(whoami)/.local/share/applications/Enigma.desktop  (app shortcut)
################################################################################
## Windows:
# C:\Program Files\Enigma\Enigma                            (executable)
# C:\Program Files\Enigma\res\                              (resources: database, config, logs, icons, ...)
# TODO                                                      (app shortcut)
################################################################################


# Include GNUInstallDirs to define standard installation directories
include(GNUInstallDirs)

# Determine the installation directories
if (WIN32)
  set(INSTALL_BASE_DIR "$ENV{ProgramFiles}/Enigma")
else ()
  # Get the user's home directory
  if (NOT DEFINED ENV{HOME})
    message(FATAL_ERROR "The HOME environment variable is not defined.")
  endif ()
  set(USER_DOT_LOCAL_DIR "$ENV{HOME}/.local")
  set(INSTALL_BASE_DIR "$ENV{HOME}/Enigma")
endif ()

# Install the executable to /home/$(whoami)/Enigma/Enigma
install(TARGETS Enigma RUNTIME DESTINATION ${INSTALL_BASE_DIR})

# Install res files to /home/$(whoami)/Enigma/res
#install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/res/
#  DESTINATION ${INSTALL_BASE_DIR}/res
#  FILES_MATCHING PATTERN "*")

# Install other resource files
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/res/
  DESTINATION ${INSTALL_BASE_DIR}/res
  FILES_MATCHING PATTERN "*"
  PATTERN "Enigma.db" EXCLUDE)  # Exclude Enigma.db database from this install command,
# we don't want to overwrite user's data on an update
# Check if Enigma.db exists and install it if it doesn't
install(CODE "
  if(NOT EXISTS \"${INSTALL_BASE_DIR}/res/database/Enigma.db\")
    message(STATUS \"Enigma.db is missing. Installing it from repository's resources...\")
    file(COPY \"${CMAKE_CURRENT_SOURCE_DIR}/res/database/Enigma.db\" DESTINATION \"${INSTALL_BASE_DIR}/res/database/\")
  else()
    message(STATUS \"Enigma.db already exists. Skipping.\")
  endif()
")

# Install .desktop file to /home/$(whoami)/Enigma/Enigma.desktop
if (UNIX AND NOT APPLE)
  # Create the .desktop file content
  file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/Enigma.desktop"
    "[Desktop Entry]
Type=Application
Name=${PROJECT_NAME}
Version=${PROJECT_VERSION}
Comment=${PROJECT_DESCRIPTION}
GenericName=${PROJECT_NAME}
Exec=${INSTALL_BASE_DIR}/Enigma
Icon=${INSTALL_BASE_DIR}/res/branding/Logo.png
Categories=Utility;Security
Terminal=false"
  )
  # Put the Enigma.desktop to /home/$(whoami)/.local/share/applications (not the usr/local/share/applications so only this user will have access to the app)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Enigma.desktop DESTINATION ${USER_DOT_LOCAL_DIR}/share/applications)

elseif (WIN32)
  # TODO create a shortcut of the app in windows
endif ()
