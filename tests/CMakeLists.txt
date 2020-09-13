add_executable(example test_example.cpp)
target_link_libraries(example gtest_main)
add_test(NAME example_test COMMAND example)
