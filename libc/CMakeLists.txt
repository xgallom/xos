file(GLOB_RECURSE files "src/*.cpp" "src/*.c")

add_library(libc STATIC ${files})
target_include_directories(libc PUBLIC include)

target_link_libraries(libc PUBLIC xos_core)

if (${XOS_BUILD_KERNEL})
    target_compile_definitions(libc PRIVATE __xos_is_libk)
    target_link_libraries(libc PRIVATE xos_kernel)
else ()
    target_compile_definitions(libc PRIVATE __xos_is_libc)
endif ()
