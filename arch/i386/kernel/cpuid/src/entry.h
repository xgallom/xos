//
// Created by xgallom on 5/22/20.
//

#ifndef _XOS_ARCH_I386_KERNEL_CPUID_SRC_ENTRY_H
#define _XOS_ARCH_I386_KERNEL_CPUID_SRC_ENTRY_H

#include <xos/cpuid/entry.h>

namespace cpuid {
    void requestVendorString(RequestType::Enum request);
    void requestOther(RequestType::Enum request);
}

#endif //_XOS_ARCH_I386_KERNEL_CPUID_SRC_ENTRY_H
