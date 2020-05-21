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


.file "isAvailable.s"
.section .text
.global isAvailable
.type isAvailable, @function
isAvailable:
    pushfl
    pushfl
    xorl $0x00200000,  (%esp)
    popfl
    pushfl
    pop %eax
    xorl (%esp), %eax
    popfl
    andl $0x00200000, %eax
    ret
.size isAvailable, . - isAvailable
