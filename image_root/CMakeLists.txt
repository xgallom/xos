# xos
# Copyright (C) 2020 Milan Gallo <gallo.milan.jr@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, version 2.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.


file(GLOB_RECURSE files "src/*")

add_custom_target(image_root
        cp -r "${CMAKE_SOURCE_DIR}/image_root/src" "${CMAKE_BINARY_DIR}/image_root_bin"
        COMMAND
        cp "${CMAKE_BINARY_DIR}/xos/xos" "${CMAKE_BINARY_DIR}/image_root_bin/xos.elf"

        COMMENT Preparing image root
        DEPENDS xos
        SOURCES ${files}
        )

add_custom_target(build_image_root
        xorriso -as mkisofs -b limine-cd.bin
            -no-emul-boot -boot-load-size 4 -boot-info-table
            --efi-boot limine-eltorito-efi.bin
            -efi-boot-part --efi-boot-image --protective-msdos-label
            "${CMAKE_BINARY_DIR}/image_root_bin" -o "${CMAKE_BINARY_DIR}/xos.iso"
        COMMAND
        limine-install "${CMAKE_BINARY_DIR}/xos.iso"

        COMMENT Building iso image
        DEPENDS image_root
        )
