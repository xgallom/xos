# XOS 
*an x86 operating system project built using C++20*

## Contents
- [1. Building](#1-building)
  - [1.1. Cross-compiling GCC](#11-cross-compiling-gcc)
  - [1.2. Building xos](#12-building-xos)
- [2. Running](#2-running)
  - [2.1. Running in QEMU](#21-running-in-qemu)
- [3. License](#3-license)

## 1. Building
I develop XOS on an Arch machine, so I don't guarantee buildability for
any of the other distributions (Debian, Ubuntu, etc.). The toolchain
I currently use is GCC 10.2.0

### 1.1. Cross-compiling GCC
#### Installing Dependencies

| Dependency | Arch package |
|------------|--------------|
| Compiler   | base-devel   |
| Make       | base-devel   |
| Bison      | base-devel   |
| Flex       | base-devel   |
| GMP        | gmp          |
| MPC        | libmpc       |
| MPFR       | mpfr         |
| Texinfo    | base-devel   |

```bash
pacman -Syu base-devel gmp libmpc mpfr
```

#### Downloading Sources
Download the newest [Binutils](https://ftp.gnu.org/gnu/binutils/?C=M;O=D)
and [GCC](https://ftp.gnu.org/gnu/gcc/?C=M;O=D) and extract them wherever.

#### Config
Set `PREFIX` to the directory you want your toolchain installed into. If
the prefix is not a root directory, you don't need sudo for make install
commands.

```bash
export PREFIX="/usr/local/gcc/i686-elf"
export TARGET="i686-elf"
export PATH="$PREFIX/bin:$PATH"
```

#### Building Binutils
`binutils` is the source directory you extracted them to.

```bash
cd binutils
mkdir build
cd build

../configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
sudo make install
```

#### Building GCC
`gcc` is the source directory you extracted them to.

```bash
cd gcc
mkdir build
cd build

../configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc
make all-target-libgcc
sudo make install-gcc
sudo make install-target-libgcc
```

### 1.2. Building xos
#### Cloning Sources
The only way currently to get the sources is via the [xos git repository](https://github.com/xgallom/xos).
You can download a zip archive or use git.

```bash
git clone git@github.com:xgallom/xos.git
```

#### Config
Set `XOS_SYSROOT` to the directory you have installed your toolchain into.

```bash
cd xos
mkdir build
cd build

XOS_BUILD_TYPE=Release XOS_ARCH=i386 XOS_SYSROOT=/usr/local/gcc/i686-elf ../configure
```

#### Building
Making a bootable iso requires GRUB v2 and xorriso.

```bash
# For building a multiboot executable
make xos

# For building an iso via grub-mkrescue
make build_image_root
```

## 2. Running
### 2.1. Running in QEMU
#### Running Multiboot Executable
```bash
qemu-system-i386 -kernel "./xos/xos"
```

#### Running GRUB Generated ISO
```bash
qemu-system-i386 -cdrom "./xos.iso"
```

## 3. License
This software is licensed with the GPLv2, available in the attached [LICENSE.md](/LICENSE.md) file.

Copyright (C) 2020 Milan Gallo
