cmake_minimum_required(VERSION 3.12)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)

# specify cross compilers and tools
SET(CMAKE_C_COMPILER_WORKS 1)
SET(CMAKE_CXX_COMPILER_WORKS 1)


project(xos C CXX ASM)
set(CMAKE_C_STANDARD gnu99)
set(CMAKE_CXX_STANDARD 17)


if (DEFINED ARCH)
    set(XOS_ARCH ${ARCH})
else ()
    set(XOS_ARCH i386)
endif ()


set(COMMON_FLAGS "-ffreestanding -Wall -Wextra -Werror")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${COMMON_FLAGS} -nostdlib -T ${CMAKE_SOURCE_DIR}/arch/${XOS_ARCH}/linker.ld")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_FLAGS} -fno-exceptions -fno-rtti")

add_subdirectory(core)
add_subdirectory(arch)
add_subdirectory(libc)
add_subdirectory(xos)
