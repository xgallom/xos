add_subdirectory(asm)
add_subdirectory(tty)
add_subdirectory(vga)

add_library(arch_kernel INTERFACE)

target_link_libraries(arch_kernel INTERFACE arch_kernel_asm arch_kernel_tty arch_kernel_vga)
