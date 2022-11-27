#ifndef EXAM_TASK
#define EXAM_TASK
#include <info.h>
#include <cr.h>
#include <pagemem.h>
#include <types.h>

#define NB_TASKS 2
#define INDEX_TASK_USER1 0
#define INDEX_TASK_USER2 1

task_t tasks[NB_TASKS];

int current_task = 0;

typedef struct
{
    uint32_t eip;
    uint32_t esp_kernel;
    uint32_t esp_user;
    uint32_t pgd;
} task_t;

void __attribute__((section(".user"))) user1();
void __attribute__((section(".user"))) user2();

void sys_counter(uint32_t *counter);
void init_tasks();

#endif