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


file(GLOB_RECURSE libk_files "src/libk/*.cpp" "src/libk/*.c")


add_library(libk STATIC ${libk_files})
target_include_directories(libk PUBLIC include)

#target_link_libraries(libk PRIVATE arch_kernel)

target_compile_definitions(libk PRIVATE __xos_is_libk)


#add_library(libc STATIC ${files})
#target_include_directories(libc PUBLIC include)
#
#target_link_libraries(libc PUBLIC core)
#
#target_compile_definitions(libc PRIVATE __xos_is_libc)
