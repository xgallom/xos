file(GLOB_RECURSE files "src/*.cpp" "src/*.c")

add_executable(xos ${files})

target_link_libraries(xos PRIVATE core arch libk)
