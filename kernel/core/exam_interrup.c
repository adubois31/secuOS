#include <exam_interrup.h>
#include <exam_segment.h>
#include <exam_task.h>
#include <exam_layout.h>
#include <intr.h>
#include <debug.h>
#include <exam_task.h>

extern tss_t *TSS;
extern int current_task_index;
extern task_t tasks[NB_TASKS];

// Syscall pour afficher la valeur du compteur
// Interface Noyau
__attribute__((naked)) __regparm__(1) void kernel_handler(int_ctx_t ctx)
{
    debug("ctx : %x\n", ctx.gpr.eax.raw);
    uint32_t counter;
    asm volatile(
        "mov %%eax, %0  \n"
        "pusha          \n"
        : "=r"(counter));

    debug("Compteur = %d", counter);
    asm volatile("popa; leave; iret");
}

// Syscall pour changer de task
__attribute__((naked)) __regparm__(1) void user_handler(uint32_t ctx)
{
    debug("Changement de task\n");
    task_t *task;

    // Sauvegarder contexte ?
    tasks[current_task_index].esp_kernel = ctx;
    asm volatile("mov (%%esp), %0"
                 : "=r"(tasks[current_task_index].esp_kernel));
    current_task_index = (current_task_index + 1) % 2;
    asm volatile("mov %0, %%esp" ::"r"(tasks[current_task_index].esp_kernel));
    task = &tasks[current_task_index];

    TSS->s0.esp = task->esp_kernel;
    TSS->eip = task->eip;

    debug("Set esp\n");
    set_esp(task->esp_kernel);
    debug("Set cr3\n");
    set_cr3(task->pgd);

    asm volatile("popa");
    asm volatile("add $8, %esp"); // skip int number end error code
    asm volatile("iret");

    debug("Fin changement de task\n");
}

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