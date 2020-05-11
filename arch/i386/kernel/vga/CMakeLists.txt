file(GLOB_RECURSE files "src/*.cpp")

add_library(xos_kernel_vga STATIC ${files})
target_include_directories(xos_kernel_vga PUBLIC include)
