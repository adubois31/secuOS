#ifndef EXAM_INTERRUP
#define EXAM_INTERRUP
#include <cr.h>
#include <info.h>

void init_all_interrup();
__regparm__(1) void kernel_handler();
__regparm__(1) void user_handler();

#endif