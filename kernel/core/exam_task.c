#include <exam_task.h>
#include <asm.h>
#include <info.h>
#include <cr.h>
#include <pagemem.h>

task_t tasks[NB_TASKS];
int current_task_index;

// Appel syst / interface user
void sys_counter(uint32_t *counter)
{
    asm volatile(
        "mov %0,%%eax\n"
        "int $80" ::"r"(*counter));
    for(int i=0;i<1000000;i++);
}

// Incrémente
__attribute__((section(".user1"))) void user1()
{
    uint32_t *counter = (uint32_t *)shm_vir_user1;
    while (1)
    {
        (*counter)++;
        for(int i=0;i<1000000;i++);
    }
}

// Display
__attribute__((section(".user2"))) void user2()
{
    uint32_t *counter = (uint32_t *)shm_vir_user2;
    while (1)
    {
        sys_counter(counter);
    }
}

void init_tasks()
{
    current_task_index = -1;
    // Initialisation tache 1
    task_t *task1 = &tasks[INDEX_TASK_USER1];
    task1->eip = (uint32_t)user1;
    task1->pgd = (uint32_t)address_PGD_usr1;
    task1->esp_kernel = (uint32_t)stack_kernel_user1;
    task1->esp_user = (uint32_t)stack_user1;
    task1->state = 0;

    // Initialisation tache 2
    task_t *task2 = &tasks[INDEX_TASK_USER2];
    task2->eip = (uint32_t)user2;
    task2->pgd = (uint32_t)address_PGD_usr2;
    task2->esp_kernel = (uint32_t)stack_kernel_user2;
    task2->esp_user = (uint32_t)stack_user2;
    task2->state = 0;
}
