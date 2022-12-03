#include <exam_task.h>
#include <exam_page.h>
#include <asm.h>

// On a choisi de mettre counter au début de la shm
// Mais on peut le modifier

uint32_t *counter = (uint32_t *)0x10000; // Il faut que ce soit l'adresse de la shared mem

void sys_counter(uint32_t *counter)
{
    asm volatile(
        "mov %0, %%eax  \n"
        "int $0x80      \n" ::"r"(counter));
}

// Incrémente
__attribute__((section(".user"))) void user1()
{
    while (1)
    {
        (*counter)++;
    }
}

// Display
__attribute__((section(".user"))) void user2()
{
    while (1)
    {
        sys_counter(counter);
    }
}

void init_tasks() // int index, uint32_t esp_kernel, uint32_t esp_user, uint32_t eip, uint32_t pgd)
{
    (*counter) = 0;
    init_individual_task(INDEX_TASK_USER1, 0x555, 0x666, 0x777, 0x888); // A modifier les params
    init_individual_task(INDEX_TASK_USER2, 0x555, 0x666, 0x777, 0x888); // A modifier les params
}

void init_individual_task(int index, uint32_t eip, uint32_t pgd, uint32_t esp_kernel, uint32_t esp_user)
{
    task_t *task = &tasks[index];
    task->eip = eip;
    task->pgd = pgd;
    task->esp_kernel = esp_kernel;
    task->esp_user = esp_user;
}