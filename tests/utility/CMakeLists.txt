add_executable(example test_example.cpp)
target_link_libraries(example gtest_main)
add_test(NAME example_test COMMAND example)

add_generic_test(example_test
                 "boo"
                 SOURCE_DIRECTORY
                     ${CMAKE_CURRENT_SOURCE_DIR}
                 SOURCES
                     PRIVATE
                         test_example.cpp
                 DEFINITIONS
                     PRIVATE
                         TestIsABadName
                     PUBLIC
                         LOOOFT
                 )
add_mjolnir_test(example)
