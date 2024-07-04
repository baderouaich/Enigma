cmake_minimum_required(VERSION 3.20)
project(imgui)

file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp") # -- Dont include backends/*, use only *.cpp instead of **.cpp (** = recursive)

add_library(${PROJECT_NAME} STATIC ${SOURCES})
add_compile_definitions(${PROJECT_NAME} PUBLIC
  IMGUI_IMPL_OPENGL_LOADER_GLAD=1
)
target_include_directories(${PROJECT_NAME}
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)
