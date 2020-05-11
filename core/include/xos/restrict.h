//
// Created by xgallom on 5/11/20.
//

#ifndef _XOS_LIBC_INCLUDE_XOS_RESTRICT_H
#define _XOS_LIBC_INCLUDE_XOS_RESTRICT_H

#include <sys/cdefs.h>

#ifdef __cplusplus
#define _Rstr __restrict
#else
#define _Rstr restrict
#endif

#endif //_XOS_LIBC_INCLUDE_XOS_RESTRICT_H
