file(GLOB_RECURSE files "src/*.cpp")

add_library(arch_kernel_vga STATIC ${files})
target_include_directories(arch_kernel_vga PUBLIC include)
