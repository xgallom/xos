file(GLOB_RECURSE files "src/*.cpp")

add_library(arch_kernel_tty STATIC ${files})
target_include_directories(arch_kernel_tty PUBLIC include)

target_link_libraries(arch_kernel_tty PRIVATE arch_kernel_asm arch_kernel_vga libk)
