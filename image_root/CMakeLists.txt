file(GLOB_RECURSE files "src/*")

add_custom_target(image_root
        cp -r "${CMAKE_SOURCE_DIR}/image_root/src" "${CMAKE_BINARY_DIR}/image_root_bin"
        COMMAND
        cp "${CMAKE_BINARY_DIR}/xos/xos" "${CMAKE_BINARY_DIR}/image_root_bin/boot/xos.bin"

        COMMENT Preparing image root
        DEPENDS xos
        SOURCES ${files}
        )

add_custom_target(build_image_root
        grub-mkrescue -o "${CMAKE_BINARY_DIR}/xos.iso" "${CMAKE_BINARY_DIR}/image_root_bin"

        COMMENT Building grub image
        DEPENDS image_root
        )
