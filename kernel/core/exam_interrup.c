#include <exam_interrup.h>
#include <exam_segment.h>
#include <exam_task.h>
#include <exam_layout.h>
#include <debug.h>
#include <exam_task.h>

extern int current_task_index;
extern task_t tasks[NB_TASKS];

// Syscall pour afficher la valeur du compteur
// Interface Noyau
__attribute__((naked)) __regparm__(1) void kernel_handler(int_ctx_t *ctx)
{
    uint32_t counter;
    asm volatile(
        "mov %%eax, %0  \n"
        "pusha          \n"
        : "=r"(counter));
    debug("counter : %d\n", counter);
    debug("%x\n", ctx->gpr.eax.raw);
    asm volatile("popa;iret");
}

// Syscall pour changer de task
__attribute__((naked)) __regparm__(1) void user_handler(int_ctx_t *ctx)
{
    debug("In scheduler with index of current task : %d\n", current_task_index);

    debug("Changement de task\n");
    task_t *task;

    if (current_task_index == -1)
    {
        current_task_index = 0;
    }
    else
    {
        // Sauvegarder contexte ?
        tasks[current_task_index].esp_kernel = (uint32_t)ctx;
        asm volatile("mov (%%esp), %0"
                     : "=r"(tasks[current_task_index].esp_kernel));

        current_task_index = (current_task_index + 1) % 2;

        asm volatile("mov %0, %%esp" ::"r"(tasks[current_task_index].esp_kernel));
    }

    task = &tasks[current_task_index];

    asm volatile(
        "mov %0, (%%esp)      \n"
        "mov %1, (%%ebp)      \n" ::"r"(task->esp_kernel),
        "r"(task->ebp));

    TSS->s0.esp = task->esp_kernel;
    // set_esp(task->esp_kernel);
    set_cr3(task->pgd);

    asm volatile(
        "push %0          \n"
        "push %1          \n"
        "pushf            \n"
        "push %2          \n"
        "push %%ebx       \n"
        "iret             \n" ::"i"(d3_sel),
        "r"(task->esp_user), "i"(c3_sel), "b"((void *)task->eip));

    // asm volatile("popa");
    // asm volatile("add $8, %esp"); // skip int number end error code
    // asm volatile("iret");
    debug("Code sale");
    asm volatile("iret");
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