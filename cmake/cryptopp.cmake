############################################################################################
# Get the CryptoPP CMakeLists.txt File for CryptoPP Package GIT_TAG Must be the Same
############################################################################################
message(CHECK_START "Fetching CryptoPP-CMAKE")
set(CRYPTOPP_CMAKE "cryptopp-cmake")
set(CRYPTOPP_GIT_TAG "CRYPTOPP_8_9_0")
set(CRYPTOPP_BUILD_TESTING OFF)
set(CRYPTOPP_INSTALL OFF)
FetchContent_Declare(${CRYPTOPP_CMAKE}
  GIT_REPOSITORY  "https://github.com/abdes/cryptopp-cmake"
  GIT_TAG         ${CRYPTOPP_GIT_TAG}
)
FetchContent_MakeAvailable(${CRYPTOPP_CMAKE})
set(CRYPTOPP "cryptopp")
if(${CRYPTOPP}_POPULATED)
  # Build shared or static library
  set(BUILD_SHARED_CRYPTOPP_OLD ${BUILD_SHARED})
  set(BUILD_SHARED OFF CACHE INTERNAL "Build CryptoPP SHARED libraries")
  message("Build CryptoPP shared lib is set to: ${BUILD_SHARED}")
  if(${BUILD_SHARED} STREQUAL "ON")
    set(CRYPTOPP "cryptopp-shared")
  else ()
    set(CRYPTOPP "cryptopp-static")
  endif()
  set(BUILD_SHARED ${BUILD_SHARED_CRYPTOPP_OLD} CACHE BOOL "Type of libraries to build" FORCE)
endif()

#FetchContent_GetProperties(${CRYPTOPP_CMAKE})
#if(NOT ${CRYPTOPP_CMAKE}_POPULATED)
#  FetchContent_Populate(${CRYPTOPP_CMAKE})
#endif()


#############################################################################################
## Get the CryptoPP Package
#############################################################################################
#message(CHECK_START "Fetching CryptoPP")
#set(CRYPTOPP "cryptopp")
#FetchContent_Declare(${CRYPTOPP}
#  GIT_REPOSITORY  "https://github.com/weidai11/cryptopp"
#  GIT_TAG         ${CRYPTOPP_GIT_TAG}
#)
#
#FetchContent_GetProperties(${CRYPTOPP})
#if(NOT ${CRYPTOPP}_POPULATED)
#  FetchContent_Populate(${CRYPTOPP})
#  # !!! IMPORTANT !!!
#  # Copy the CMakeLists.txt file from https://github.com/noloader/cryptopp-cmake with same TAG CRYPTOPP repository into ${${CRYPTOPP}_SOURCE_DIR}
#  # before using add_subdirectory(), include_directories() and set(CRYPTOPP_LIB....) commands, until the a proper COPY command was implemented.
#  file(COPY ${${CRYPTOPP_CMAKE}_SOURCE_DIR}/CMakeLists.txt DESTINATION ${${CRYPTOPP}_SOURCE_DIR})
#  file(COPY ${${CRYPTOPP_CMAKE}_SOURCE_DIR}/cmake DESTINATION ${${CRYPTOPP}_SOURCE_DIR})
#  add_subdirectory(${${CRYPTOPP}_SOURCE_DIR} ${${CRYPTOPP}_BINARY_DIR})
#endif()
#include_directories(${${CRYPTOPP}_SOURCE_DIR})
#
## Set Cryptopp library properties every time after the first population
#if(${CRYPTOPP}_POPULATED)
#  # Build shared or static library
#  set(BUILD_SHARED_CRYPTOPP_OLD ${BUILD_SHARED})
#  set(BUILD_SHARED OFF CACHE INTERNAL "Build CryptoPP SHARED libraries")
#  message("Build CryptoPP shared lib is set to: ${BUILD_SHARED}")
#  if(${BUILD_SHARED} STREQUAL "ON")
#    set(CRYPTOPP "cryptopp-shared")
#  else ()
#    set(CRYPTOPP "cryptopp-static")
#  endif()
#  set(BUILD_SHARED ${BUILD_SHARED_CRYPTOPP_OLD} CACHE BOOL "Type of libraries to build" FORCE)
#endif()