//
// Created by xgallom on 5/22/20.
//

#include <xos/cpuid/feature.h>
#include <xos/cpuid/entry.h>

namespace cpuid {
    const char *FeatureTypeNameMap[2][32] = {
	    {
		    "SSE3 PNI",
		    "Carry-less Multiplication",
		    "64-bit Debug Store",
		    "MONITOR and MWAIT (SSE3)",
		    "CPL Qualified Debug Store",
		    "Virtual Machine eXtensions",
		    "Safer Mode Extensions",
		    "Enhanced SpeedStep",
		    "Thermal Monitor 2",
		    "Supplemental SSE3 ",
		    "L1 Context ID",
		    "Silicon Debug interface",
		    "Fused Multiply-Add",
		    "CMPXCHG16B",
		    "Disable Sending Task Priority Messages",
		    "Perfmon and Debug Capability",
		    "_reserved_ecx16",
		    "Process Context Identifiers",
		    "Direct Cache Access for DMA Writes",
		    "SSE 4.1",
		    "SSE 4.2",
		    "x2APIC",
		    "Move Big-Endian",
		    "Population Count",
		    "TSC Deadline for APIC One Shot",
		    "AES",
		    "XSAVE, XRESTOR, SETBV, SGETBV",
		    "OS Enabled XSAVE",
		    "AVX",
		    "Half Precision Floating Point",
		    "On-Chip RNG",
		    "Hypervisor",
	    },
	    {
		    "x87 FPU",
		    "Virual 8086 Mode Extensions",
		    "Debug Extensions",
		    "Page Size Extension",
		    "Time Stamp Counter",
		    "Model-Specific Registers",
		    "Physical Address Extension",
		    "Machine Check Exception",
		    "Compare and Swap Instruction",
		    "Advanced Programmable Interrupt Controller",
		    "_reserved_edx10",
		    "SYSENTER and SYSEXIT",
		    "Memory Type Range Registers",
		    "Page Global Enable",
		    "Machine Check Architecture",
		    "Conditional Move Instructions",
		    "Page Attribute Table",
		    "36-bit Page Size Extension",
		    "Processor Serial Number",
		    "CLFLUSH (SSE2)",
		    "_reserved_edx20",
		    "Debug Store",
		    "ACPI",
		    "MMX",
		    "FXSAVE and FXRESTOR",
		    "SSE",
		    "SSE2",
		    "CPU Cache Self-Snoop",
		    "Hyper-Threading",
		    "Automatic Temperature Limiting",
		    "IA64 x86 Emulation",
		    "Pending Break Enable",
	    }
    };

    bool feature(FeatureType::Enum feature)
    {
	    const bool isEdx = (feature & FeatureType::EdxMask) != 0u;
	    const uint8_t offset = 2 + isEdx;
	    const uint32_t
		    featureEntry = entry(RequestType::GetFeatures)[offset],
		    featureMask = 1u << (feature & FeatureType::BitOffsetMask);

	    return (featureEntry & featureMask) != 0;
    }

    const char *featureName(FeatureType::Enum feature)
    {
	    const bool isEdx = (feature & FeatureType::EdxMask) != 0u;
	    const uint8_t offset = feature & FeatureType::BitOffsetMask;

	    return FeatureTypeNameMap[isEdx][offset];
    }
}
