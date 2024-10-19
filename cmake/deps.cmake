include(FetchContent)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
set(BUILD_STATIC_LIBS ON CACHE BOOL "Build static libraries" FORCE)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/cryptopp.cmake)

FetchContent_Declare(cpr
  GIT_REPOSITORY "https://github.com/libcpr/cpr"
  GIT_TAG "1.11.0"
  EXCLUDE_FROM_ALL # to exclude this dependency from being installed with Enigma install target
)
set(BUILD_CURL_EXE OFF CACHE BOOL "Set to ON to build curl executable." FORCE)
set(BUILD_STATIC_CURL ON CACHE BOOL "Build curl executable with static libcurl" FORCE)
FetchContent_MakeAvailable(cpr)


# CLI not supported
#FetchContent_Declare(cxxopts
#  GIT_REPOSITORY "https://github.com/jarro2783/cxxopts"
#  GIT_TAG "v3.2.0"
#  EXCLUDE_FROM_ALL # to exclude this dependency from being installed with Enigma install target
#)
#FetchContent_MakeAvailable(cxxopts)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/glad EXCLUDE_FROM_ALL)


FetchContent_Declare(glfw
  GIT_REPOSITORY "https://github.com/glfw/glfw"
  GIT_TAG "3.4"
  EXCLUDE_FROM_ALL # to exclude this dependency from being installed with Enigma install target

)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)


add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui EXCLUDE_FROM_ALL)

FetchContent_Declare(infoware
  GIT_REPOSITORY "https://github.com/ThePhD/infoware"
  GIT_TAG "v0.6.0"
  EXCLUDE_FROM_ALL # to exclude this dependency from being installed with Enigma install target
)
if (UNIX AND NOT APPLE)
  set(INFOWARE_USE_X11 ON CACHE BOOL "" FORCE)
endif ()
FetchContent_MakeAvailable(infoware)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/inih EXCLUDE_FROM_ALL)


# https://github.com/nlohmann/json?tab=readme-ov-file#embedded-fetchcontent
# Avoid warning about DOWNLOAD_EXTRACT_TIMESTAMP in CMake 3.24:
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
  cmake_policy(SET CMP0135 NEW)
endif ()
FetchContent_Declare(json
  URL "https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz"
  EXCLUDE_FROM_ALL # to exclude this dependency from being installed with Enigma install target
)
FetchContent_MakeAvailable(json)


FetchContent_Declare(portable_file_dialogs
  GIT_REPOSITORY "https://github.com/samhocevar/portable-file-dialogs"
  GIT_TAG "0.1.0"
  EXCLUDE_FROM_ALL # to exclude this dependency from being installed with Enigma install target
)
FetchContent_MakeAvailable(portable_file_dialogs)

FetchContent_Declare(spdlog
  GIT_REPOSITORY "https://github.com/gabime/spdlog"
  GIT_TAG "v1.8.2"
  EXCLUDE_FROM_ALL # to exclude this dependency from being installed with Enigma install target
)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(SQLiteCpp
  GIT_REPOSITORY "https://github.com/SRombauts/SQLiteCpp"
  GIT_TAG "3.3.2"
  EXCLUDE_FROM_ALL # to exclude this dependency from being installed with Enigma install target

)
set(SQLITECPP_RUN_CPPLINT OFF)
FetchContent_MakeAvailable(SQLiteCpp)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/stb_image EXCLUDE_FROM_ALL)

# CLI not supported
#FetchContent_Declare(tabulate
#  GIT_REPOSITORY "https://github.com/p-ranav/tabulate"
#  GIT_TAG "v1.5"
#  EXCLUDE_FROM_ALL # to exclude this dependency from being installed with Enigma install target
#)
#FetchContent_MakeAvailable(tabulate)

FetchContent_Declare(zlib
  GIT_REPOSITORY "https://github.com/madler/zlib"
  GIT_TAG "v1.3.1"
  EXCLUDE_FROM_ALL # to exclude this dependency from being installed with Enigma install target
)
FetchContent_MakeAvailable(zlib)

if (ENIGMA_ENABLE_TESTS)
  set(CATCH_INSTALL_DOCS OFF CACHE BOOL "" FORCE)
  set(CATCH_INSTALL_EXTRAS OFF CACHE BOOL "" FORCE)
  set(CATCH_DEVELOPMENT_BUILD OFF CACHE BOOL "" FORCE)
  set(CATCH_ENABLE_REPRODUCIBLE_BUILD OFF CACHE BOOL "" FORCE)
  FetchContent_Declare(Catch2
    GIT_REPOSITORY "https://github.com/catchorg/Catch2"
    GIT_TAG "v3.7.1"
    EXCLUDE_FROM_ALL # to exclude this dependency from being installed with Enigma install target
  )
  FetchContent_MakeAvailable(Catch2)
	# Force Catch2 to use C++20 to avoid MSVC issue (unresolved external related to std::string_view c++14)
	# see catch2 issue #2046
	if ("cxx_std_20" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
			target_compile_features(Catch2 PRIVATE cxx_std_20)
			target_compile_features(Catch2WithMain PRIVATE cxx_std_20)
	endif()
endif ()

set(ENIGMA_DEPENDENCIES
  cpr
  ${CRYPTOPP}
  glad
  glfw
  imgui
  infoware
  inih
  nlohmann_json::nlohmann_json
  portable_file_dialogs
  spdlog
  #cxxopts
  #tabulate
  SQLiteCpp
  stb_image
  zlib
)
# Append Ws2_32 if we're on windows for the SingleProcessInstance socket
if (WIN32)
  set(ENIGMA_DEPENDENCIES ${ENIGMA_DEPENDENCIES} Ws2_32)
endif ()

# Append Catch2 if we're testing
if (ENIGMA_ENABLE_TESTS)
  set(ENIGMA_DEPENDENCIES ${ENIGMA_DEPENDENCIES} Catch2::Catch2WithMain)
endif ()
