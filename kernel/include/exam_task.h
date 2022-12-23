#ifndef EXAM_TASK
#define EXAM_TASK
#include <types.h>
#include <exam_layout.h>
#include <intr.h>

#define NB_TASKS 2
#define INDEX_TASK_USER1 0
#define INDEX_TASK_USER2 1
typedef struct
{
    uint32_t eip;
    uint32_t ebp;
    uint32_t esp_kernel;
    uint32_t esp_user;
    uint32_t pgd;
    int_ctx_t ctx_task;
    int state;
} task_t;

extern task_t tasks[NB_TASKS];
extern int current_task_index;

void __attribute__((section(".user1"))) user1();
void __attribute__((section(".user2"))) user2();

void sys_counter(uint32_t *counter);
void init_tasks();

#endif