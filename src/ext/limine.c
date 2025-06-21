#include <stddef.h>

#define LIMINE_API_REVISION 3
#include "../../limine/limine.h"

LIMINE_REQUESTS_START_MARKER
LIMINE_BASE_REVISION(3)

struct limine_stack_size_request limine_stack_size_request = {
	.id = LIMINE_STACK_SIZE_REQUEST,
	.revision = 0,
	.response = NULL,
	.stack_size = 1 << 20,
};

struct limine_framebuffer_request limine_framebuffer_request = {
	.id = LIMINE_FRAMEBUFFER_REQUEST,
	.revision = 1,
	.response = NULL,
};

LIMINE_REQUESTS_END_MARKER

