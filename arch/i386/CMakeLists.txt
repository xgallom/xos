add_subdirectory(boot)
add_subdirectory(kernel)

add_library(arch INTERFACE)
target_link_libraries(arch INTERFACE boot xos_kernel)
