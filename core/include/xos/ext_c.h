//
// Created by xgallom on 5/11/20.
//

#ifndef _XOS_LIBC_INCLUDE_XOS_EXT_C_H
#define _XOS_LIBC_INCLUDE_XOS_EXT_C_H

#include <sys/cdefs.h>

#ifdef __cplusplus

#define _EXT_C extern "C" {
#define _EXT_C_END }

#else

#define _EXT_C
#define _EXT_C_END

#endif

#endif //_XOS_LIBC_INCLUDE_XOS_EXT_C_H
