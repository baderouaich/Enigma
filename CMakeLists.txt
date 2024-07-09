cmake_minimum_required(VERSION 3.20)
project(Enigma VERSION "3.1.0" DESCRIPTION "A Simple, Reliable and Efficient Encryption Tool" LANGUAGES CXX)

################# Options ###########################
option(ENIGMA_ENABLE_TESTS "Enable/Disable building of tests" OFF)
option(ENIGMA_BUILD_DOCS "Build Doxygen API documentation" OFF)
#####################################################

###### Doxygen Documentation ######
if (ENIGMA_BUILD_DOCS)
  message(STATUS "Building Doxygen docs is enabled")
  find_package(Doxygen REQUIRED)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
  add_custom_target(enigma_build_docs ALL
    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating Doxygen API documentation..."
    VERBATIM
  )
  install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs/html/ TYPE DOC)
  # No need to go further as we're only building docs.
  return()
endif ()
###################################


################## Libraries ##########################
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/deps.cmake")
#######################################################


################## Main Project ##########################
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/**.cpp")
if(ENIGMA_ENABLE_TESTS)
  # Tests enabled? Build Enigma as a static lib so EnigmaTests will link with it.
  add_library(${PROJECT_NAME} STATIC ${SOURCES})
else ()
  # Tests off? build Enigma as an application
  add_executable(${PROJECT_NAME} ${SOURCES})
endif ()
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_precompile_headers(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/pch.hpp")
target_compile_definitions(${PROJECT_NAME} PRIVATE
  $<$<CONFIG:Debug>:ENIGMA_DEBUG>
  $<$<CONFIG:Release>:ENIGMA_RELEASE>
)
target_compile_definitions(${PROJECT_NAME} PUBLIC
  ENIGMA_VERSION="${PROJECT_VERSION}"
  ENIGMA_VERSION_MINOR=${PROJECT_VERSION_MINOR}
  ENIGMA_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
  ENIGMA_VERSION_PATCH=${PROJECT_VERSION_PATCH}
  ENIGMA_DESCRIPTION="${PROJECT_DESCRIPTION}"
  LOCAL_RES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/res"
#  CONFIG_DIR="${CMAKE_CURRENT_SOURCE_DIR}/res/config"
#  FONTS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/res/fonts"
#  BRANDING_DIR="${CMAKE_CURRENT_SOURCE_DIR}/res/branding"
#  DATABASE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/res/database"
#  LOGS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/res/logs"
)
target_link_libraries(${PROJECT_NAME} PRIVATE
  ${ENIGMA_DEPENDENCIES}
)
####################################################


############ Platform macros ############
if (WIN32)
  # No need, will be defined by Core/Platform.hpp
  # target_compile_definitions(${PROJECT_NAME} PRIVATE ENIGMA_PLATFORM_WINDOWS)

  # Use Enigma.rc resource file
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Enigma.rc.in ${CMAKE_CURRENT_BINARY_DIR}/Enigma.rc)
  target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/Enigma.rc)

  # Common MSVC warnings defines
  if (MSVC)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
      _CRT_SECURE_NO_DEPRECATE
      _CRT_SECURE_NO_WARNINGS
      _CRT_NONSTDC_NO_WARNINGS
    )
  elseif (MINGW)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
      STRSAFE_NO_DEPRECATE # for strcpy_instead_use_StringCbCopyA_or_StringCchCopyA
    )
  endif ()
elseif (UNIX AND NOT APPLE)
  # No need, will be defined by Core/Platform.hpp
  # target_compile_definitions(${PROJECT_NAME} PRIVATE ENIGMA_PLATFORM_LINUX)
else ()
  message(FATAL_ERROR "Unsupported platform")
endif ()
########################################


###### Tests ######
if (ENIGMA_ENABLE_TESTS)
  message(STATUS "Building tests is enabled")
  enable_testing()
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif ()
####################


#### Install scripts ######
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/install.cmake")
