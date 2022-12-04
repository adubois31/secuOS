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
    // debug("---Display GDT---\n");
    // display_gdt();
    // while (1)
    // {
    // }

    debug("---Initialisation Interruptions---\n");
    init_all_interrup();

    debug("---Initialisation Tâches---\n");
    init_tasks();

    debug("---Initialisation Pages---\n");
    page_init();

    // debug("---Display PGD User1---\n");
    // display_pgd(address_PGD_usr1);

    force_interrupts_on();
    //  asm volatile("int $80");
    // uint32_t *var = (uint32_t *)0x890000;
    // *var = 78;
    // // asm volatile("int $32");
    // sys_counter(var);
    while (1)
    {
    }
}
