
file(GLOB_RECURSE TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/**.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/**.hpp")
add_executable(EnigmaTests ${TEST_SOURCES})
target_compile_features(EnigmaTests PRIVATE cxx_std_20)
target_link_libraries(EnigmaTests PRIVATE Enigma ${ENIGMA_DEPENDENCIES})
target_include_directories(EnigmaTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/src)
target_compile_definitions(EnigmaTests PRIVATE
  TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/test_data"
)