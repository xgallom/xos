#include <stddef.h>

#define LIMINE_API_REVISION 3
#include "../../limine/limine.h"

LIMINE_REQUESTS_START_MARKER
LIMINE_BASE_REVISION(3)

struct limine_bootloader_info_request limine_bootloader_info_request = {
	.id = LIMINE_BOOTLOADER_INFO_REQUEST,
	.revision = 0,
	.response = NULL,
};

struct limine_executable_cmdline_request limine_executable_cmdline_request = {
	.id = LIMINE_EXECUTABLE_CMDLINE_REQUEST,
	.revision = 0,
	.response = NULL,
	
};

struct limine_firmware_type_request limine_firmware_type_request = {
	.id = LIMINE_FIRMWARE_TYPE_REQUEST,
	.revision = 0,
	.response = NULL,
};

struct limine_stack_size_request limine_stack_size_request = {
	.id = LIMINE_STACK_SIZE_REQUEST,
	.revision = 0,
	.response = NULL,
	.stack_size = 1 << 20,
};

struct limine_hhdm_request limine_hhdm_request = {
	.id = LIMINE_HHDM_REQUEST,
	.revision = 0,
	.response = NULL,
};

struct limine_framebuffer_request limine_framebuffer_request = {
	.id = LIMINE_FRAMEBUFFER_REQUEST,
	.revision = 1,
	.response = NULL,
};

struct limine_memmap_request limine_memmap_request = {
	.id = LIMINE_MEMMAP_REQUEST,
	.revision = 0,
	.response = NULL,
};

struct limine_executable_file_request limine_executable_file_request = {
	.id = LIMINE_EXECUTABLE_FILE_REQUEST,
	.revision = 0,
	.response = NULL,
};

LIMINE_REQUESTS_END_MARKER

