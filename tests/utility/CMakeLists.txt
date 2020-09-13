add_executable(example test_example.cpp)
target_link_libraries(example gtest_main)
add_test(NAME example_test COMMAND example)

add_generic_test(example_test ${CMAKE_CURRENT_SOURCE_DIR} "boo" test_example.cpp)
add_mjolnir_test(example)
