message(">>> Test will be built")
# add_executable(test op2D_test.cpp)
# target_link_libraries(test oplib)

add_executable(example op2D_example.cpp )
target_link_libraries(example oplib)