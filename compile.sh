#!/bin/bash

nasm -o bin/img src/src.asm
nasm -o bin/disk src/disk.asm

dd if=bin/img of=bin/image.img conv=notrunc
dd if=bin/disk of=bin/disk.img conv=notrunc

bochs

