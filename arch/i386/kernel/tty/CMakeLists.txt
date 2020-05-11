file(GLOB_RECURSE files "src/*.cpp")

add_library(xos_kernel_tty STATIC ${files})
target_include_directories(xos_kernel_tty PUBLIC include)

target_link_libraries(xos_kernel_tty PUBLIC xos_kernel_vga libc)
