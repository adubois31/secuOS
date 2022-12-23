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

    // Passage ring 0->3
    if (current_task_index == -1)
    {
        current_task_index = 0;
        set_cr3(tasks[0].pgd);
        asm volatile(
            "push %0          \n"
            "push %1          \n"
            "pushf            \n"
            "push %2          \n"
            "push %%ebx       \n" ::"i"(d3_sel),
            "r"(task->esp_user),
            "i"(c3_sel),
            "b"((void *)task->eip));
        asm volatile("iret");
    }
    else
    {
        current_task_index = (current_task_index + 1) % 2;
        task = &tasks[current_task_index];
        if (task->state == 0)
        {
            ctx->esp = task->esp_user;
            ctx->eip = task->eip;
            task->state = 1;
        }
        else
        {
            tasks[(current_task_index + 1) % 2].esp_kernel = (uint32_t)ctx;
            *ctx = task->esp_kernel;
        }
        tss_t *TSS = (tss_t *)address_TSS;
        TSS->s0.esp = task->esp_kernel;
        set_esp(task->esp_kernel);
        set_cr3(task->pgd);
        asm volatile("iret");
    }
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