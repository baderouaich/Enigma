cmake_minimum_required(VERSION 3.20)
project(imgui)

file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp") # -- Dont include backends/*, use only *.cpp instead of **.cpp (** = recursive)

#set(SOURCES
#    "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"
#    "${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_glfw.cpp"
#    "${CMAKE_CURRENT_SOURCE_DIR}/imgui.cpp"
#    "${CMAKE_CURRENT_SOURCE_DIR}/imgui_draw.cpp"
#    "${CMAKE_CURRENT_SOURCE_DIR}/imgui_tables.cpp"
#    "${CMAKE_CURRENT_SOURCE_DIR}/imgui_widgets.cpp"
#)
add_compile_definitions(
        -DIMGUI_IMPL_OPENGL_LOADER_CUSTOM=1
        -DIMGUI_IMPL_OPENGL_LOADER_GLAD=1
)
add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_compile_definitions(${PROJECT_NAME} PUBLIC
  IMGUI_IMPL_OPENGL_LOADER_GLAD=1
)
target_include_directories(${PROJECT_NAME}
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)
