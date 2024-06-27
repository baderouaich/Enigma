include(FetchContent)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/cryptopp.cmake)

FetchContent_Declare(Catch2
        GIT_REPOSITORY "https://github.com/catchorg/Catch2"
        GIT_TAG        "v3.6.0"
)
FetchContent_MakeAvailable(Catch2)

FetchContent_Declare(cpr
        GIT_REPOSITORY "https://github.com/libcpr/cpr"
        GIT_TAG        "1.10.5"
)
FetchContent_MakeAvailable(cpr)


FetchContent_Declare(cxxopts
        GIT_REPOSITORY "https://github.com/jarro2783/cxxopts"
        GIT_TAG        "v3.2.0"
)
FetchContent_MakeAvailable(cxxopts)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/glad)

FetchContent_Declare(glfw
        GIT_REPOSITORY "https://github.com/glfw/glfw"
        GIT_TAG        "3.4"
)
FetchContent_MakeAvailable(glfw)


add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui)

FetchContent_Declare(infoware
        GIT_REPOSITORY "https://github.com/ThePhD/infoware"
        GIT_TAG        "v0.6.0"
)
FetchContent_MakeAvailable(infoware)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/inih)


# https://github.com/nlohmann/json?tab=readme-ov-file#embedded-fetchcontent
# Avoid warning about DOWNLOAD_EXTRACT_TIMESTAMP in CMake 3.24:
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()
FetchContent_Declare(json
        URL "https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz"
)
FetchContent_MakeAvailable(json)

#FetchContent_Declare(mbedtls
#  GIT_REPOSITORY "https://github.com/Mbed-TLS/mbedtls"
#  GIT_TAG        "v3.6.0"
#)
#FetchContent_MakeAvailable(mbedtls)

FetchContent_Declare(portable_file_dialogs
        GIT_REPOSITORY "https://github.com/samhocevar/portable-file-dialogs"
        GIT_TAG        "0.1.0"
)
FetchContent_MakeAvailable(portable_file_dialogs)

FetchContent_Declare(spdlog
        GIT_REPOSITORY "https://github.com/gabime/spdlog"
        GIT_TAG        "v1.8.2"
)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(SQLiteCpp
        GIT_REPOSITORY "https://github.com/SRombauts/SQLiteCpp"
        GIT_TAG        "3.3.1"
)
set(SQLITECPP_RUN_CPPLINT OFF)
FetchContent_MakeAvailable(SQLiteCpp)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/stb_image)

FetchContent_Declare(tabulate
        GIT_REPOSITORY "https://github.com/p-ranav/tabulate"
        GIT_TAG        "v1.5"
)
FetchContent_MakeAvailable(tabulate)

FetchContent_Declare(zlib
        GIT_REPOSITORY "https://github.com/madler/zlib"
        GIT_TAG        "v1.3.1"
)
FetchContent_MakeAvailable(zlib)


set(ENIGMA_DEPENDENCIES
        cpr
        ${CRYPTOPP}
        glad
        cxxopts
        glfw
        imgui
        infoware
        inih
        nlohmann_json::nlohmann_json
        #mbedtls
        portable_file_dialogs
        spdlog
        SQLiteCpp
        stb_image
        tabulate
        zlib
)