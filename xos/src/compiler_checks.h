//
// Created by xgallom on 5/11/20.
//

#ifndef XOS_XOS_SRC_COMPILER_CHECKS_H
#define XOS_XOS_SRC_COMPILER_CHECKS_H

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__) || \
	defined(unix) || defined(__unix) || defined(__unix__) || \
	defined(_WIN32) || defined(WIN32) || \
	defined(__APPLE__) || defined(__MACH__) || \
	defined(__FreeBSD__) || \
	defined(__ANDROID__)
#error "Can't use the system compiler for compiling xos"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "xos requires an ix86-elf compiler"
#endif

#endif //XOS_XOS_SRC_COMPILER_CHECKS_H
