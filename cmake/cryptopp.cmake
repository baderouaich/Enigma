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
  EXCLUDE_FROM_ALL # to exclude this dependency from being installed with Enigma install target
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

# Enable namespace CryptoPP::Weak::* to use weak algorithms such as MD2 MD4..
target_compile_definitions(${CRYPTOPP} PUBLIC CRYPTOPP_ENABLE_NAMESPACE_WEAK=1)