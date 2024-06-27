file(GLOB TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
## CTest
foreach (source ${TEST_SOURCES})
  get_filename_component(filename ${source} NAME_WE)
  add_executable(${filename} ${source})
  add_test(${filename} ${filename}) # add target to CTest flow
  target_compile_features(${filename} PRIVATE cxx_std_20)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/catch2/include)
  # Link the tests with the main project tgbotxx
  target_link_libraries(${filename} PRIVATE tgbotxx)
  if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    # valgrind memory leak checks (NOTE: valgrind has problems with clang symbols, compile with gcc before running this target)
    add_custom_target(check_memory_leaks_${filename}
      COMMAND valgrind --tool=memcheck --leak-check=full -s --show-leak-kinds=all --track-origins=yes ${CMAKE_CURRENT_BINARY_DIR}/${filename}
    )
  endif ()
endforeach ()

## Manual Tests
add_executable(manual_tests manual_tests.cpp)
target_compile_features(manual_tests PRIVATE cxx_std_20)
target_link_libraries(manual_tests PRIVATE tgbotxx ${TGBOTXX_LIBRARIES})
#target_compile_definitions(manual_tests PRIVATE RES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/res")
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests_bot_token.sh)
  add_custom_command(
    TARGET manual_tests PRE_BUILD
    COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/tests_bot_token.sh
    VERBATIM Sourcing ${CMAKE_CURRENT_SOURCE_DIR}/tests_bot_token.sh ...
  )
else ()
  message(WARNING "Missing tests bot token file ${CMAKE_CURRENT_SOURCE_DIR}/tests_bot_token.sh, please create file and add an export TESTS_BOT_TOKEN=YOUR TESTING BOT TOKEN")
endif ()

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  # valgrind memory leak checks (NOTE: valgrind has problems with clang symbols, compile with gcc before running this target)
  add_custom_target(check_memory_leaks_manual_tests
    COMMAND valgrind --tool=memcheck --leak-check=full -s --show-leak-kinds=all --track-origins=yes ${CMAKE_CURRENT_BINARY_DIR}/manual_tests
  )
endif ()