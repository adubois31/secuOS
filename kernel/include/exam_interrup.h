#ifndef EXAM_INTERRUP
#define EXAM_INTERRUP
#include <cr.h>
#include <info.h>
#include <exam_segment.h>
#include <exam_task.h>

void init_kernel_interrup();
void init_user_interrup();

void kernel_handler();
void user_handler();

#endif