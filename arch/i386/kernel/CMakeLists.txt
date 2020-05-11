add_subdirectory(tty)
add_subdirectory(vga)

add_library(xos_kernel INTERFACE)

target_link_libraries(xos_kernel INTERFACE xos_kernel_tty xos_kernel_vga)
