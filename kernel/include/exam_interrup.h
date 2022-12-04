#ifndef EXAM_INTERRUP
#define EXAM_INTERRUP
#include <cr.h>
#include <intr.h>
#include <info.h>
#include <types.h>
#include <intr.h>

void init_all_interrup();
void user_handler(int_ctx_t *) __regparm__(1);
void kernel_handler(int_ctx_t *ctx) __regparm__(1);
#endif