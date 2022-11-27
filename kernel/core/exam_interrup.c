#include <exam_interrup.h>
#include <segmem.h>
#include <intr.h>
#include <debug.h>
#include <exam_task.h>

#include <exam_segment.h>

void init_interrup(int num_inter, int privilege, offset_t handler)
{
    idt_reg_t idtr;
    get_idtr(idtr);

    int_desc_t *iface_dsc = &idtr.desc[num_inter];
    int_desc(iface_dsc, c0_sel, handler);
    iface_dsc->type = SEG_DESC_SYS_TRAP_GATE_32;
    iface_dsc->dpl = privilege;
}

void init_all_interrup()
{
    init_interrup(32, 0, (offset_t)user_handler);
    init_interrup(80, 3, (offset_t)kernel_handler);
}

// Syscall pour afficher la valeur du compteur
__attribute__((naked)) void kernel_handler()
{
    uint32_t counter;
    asm volatile(
        "mov %%eax, %0  \n"
        "pusha          \n"
        : "=r"(counter));

    // debug("Compteur = %d",counter);
    asm volatile("popa; leave; iret");
}

// Syscall pour changer de task
__attribute__((naked)) void user_handler()
{
    // Affecter ces variables avec quelque chose
    task_t *task = &task[current_task];
    tss_t *tss;
    tss->s0.esp;
    tss->s0.ss;
    set_esp();
    set_cr3(task->pgd);
    asm volatile("popa");         // pop general registers and EBP
    asm volatile("add $8, %esp"); // skip int number end error code
    asm volatile("iret");
    // AU secours
}
