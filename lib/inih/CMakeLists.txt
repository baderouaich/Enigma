cmake_minimum_required(VERSION 3.20)
project(inih)

add_library(${PROJECT_NAME} STATIC ${CMAKE_CURRENT_SOURCE_DIR}/ini.c  ${CMAKE_CURRENT_SOURCE_DIR}/cpp/INIReader.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/cpp)