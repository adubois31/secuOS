/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>
#include <asm.h>

#include <exam_segment.h>
#include <exam_page.h>
#include <exam_task.h>
#include <exam_interrup.h>
#include <asm.h>

extern info_t *info;
extern uint32_t __kernel_start__;
extern uint32_t __kernel_end__;

void tp()
{

    debug("kernel mem [0x%x - 0x%x]\n", (uint32_t)&__kernel_start__, (uint32_t)&__kernel_end__);
    debug("MBI flags 0x%x\n\n", info->mbi->flags);

    debug("---Initialisation GDT---\n");
    init_gdt();

    debug("---Initialisation Interruptions---\n");
    init_all_interrup();

    debug("---Initialisation Tâches---\n");
    init_tasks();

    debug("---Initialisation Pages---\n");
    page_init();

    current_task_index = 0;
    task_t *task = &tasks[current_task_index];
    set_cr3(task->pgd);

    asm volatile(
        "push %0          \n"
        "push %1          \n"
        "pushf            \n"
        "pop %%eax        \n"
        "or $0x200, %%eax\n"
        "push %%eax\n"
        "push %2          \n"
        "push %%ebx       \n" ::"i"(d3_sel),
        "r"(task->esp_user),
        "i"(c3_sel),
        "b"((void *)task->eip));
    asm volatile("iret");
}
