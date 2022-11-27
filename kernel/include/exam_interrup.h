#ifndef EXAM_INTERRUP
#define EXAM_INTERRUP
#include <cr.h>

void init_kernel_interrup();
void init_user_interrup();

void kernel_handler();
void user_handler();

#endif