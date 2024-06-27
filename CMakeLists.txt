cmake_minimum_required(VERSION 3.20)
project(Enigma VERSION "3.0.0" DESCRIPTION "A Simple, Reliable and Efficient UI + CLI Encryption Tool" LANGUAGES CXX)

################# Options ###########################
option(ENIGMA_ENABLE_TESTS "Enable/Disable building of tests" OFF)
option(ENIGMA_BUILD_DOCS "Build doxygen API documentation" OFF)
#####################################################

################## Libraries ##########################
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/deps.cmake")
#######################################################


################## Main Project ##########################
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/**.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_precompile_headers(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/pch.hpp")
target_compile_definitions(${PROJECT_NAME} PRIVATE
  $<$<CONFIG:Debug>:ENIGMA_DEBUG>
  $<$<CONFIG:Release>:ENIGMA_RELEASE>
)
target_compile_definitions(${PROJECT_NAME} PRIVATE
  ENIGMA_VERSION="${PROJECT_VERSION}"
  ENIGMA_DESCRIPTION="${PROJECT_DESCRIPTION}"
  RES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/res"
  CONFIG_DIR="${CMAKE_CURRENT_SOURCE_DIR}/res/config"
  FONTS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/res/fonts"
  BRANDING_DIR="${CMAKE_CURRENT_SOURCE_DIR}/res/branding"
  DATABASE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/res/database"
  LOGS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/res/logs"
)
target_link_libraries(${PROJECT_NAME} PRIVATE
  ${ENIGMA_DEPENDENCIES}
)
####################################################


############ Platform macros ############
if(WIN32)
  target_compile_definitions(${PROJECT_NAME} PRIVATE ENIGMA_PLATFORM_WINDOWS)
elseif (UNIX AND NOT APPLE)
  target_compile_definitions(${PROJECT_NAME} PRIVATE ENIGMA_PLATFORM_LINUX)
else()
  message(FATAL_ERROR "Unsupported platform")
endif()
########################################


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
endif ()
###################################


###### Tests ######
if (ENIGMA_ENABLE_TESTS)
  message(STATUS "Building tests is enabled")
# to add in tests/Cmakelists.txt  target_link_libraries(${PROJECT_NAME} PRIVATE
#    Catch2::Catch2WithMain
#  )
  enable_testing()
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif ()
####################