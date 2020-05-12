file(GLOB_RECURSE files "src/*.cpp" "src/*.c")


add_library(libk STATIC ${files})
target_include_directories(libk PUBLIC include)

target_link_libraries(libk PUBLIC core)
target_link_libraries(libk PRIVATE arch_kernel)

target_compile_definitions(libk PRIVATE __xos_is_libk)


add_library(libc STATIC ${files})
target_include_directories(libc PUBLIC include)

target_link_libraries(libc PUBLIC core)

target_compile_definitions(libc PRIVATE __xos_is_libc)
