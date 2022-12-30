#include <exam_interrup.h>
#include <exam_segment.h>
#include <exam_task.h>
#include <exam_layout.h>
#include <debug.h>

extern int current_task_index;
extern task_t tasks[NB_TASKS];
extern void user_intr();

// Syscall pour afficher la valeur du compteur
// Interface Noyau
void kernel_handler()
{
    uint32_t counter;
    asm volatile(
        "pusha          \n"
        "mov %%eax, %0  \n"
        : "=r"(counter));
    debug("Counter : %d\n", counter);
    asm volatile("popa");
}

// Syscall pour changer de task
__regparm__(1) void user_handler(int_ctx_t *ctx)
{
    debug("Interruption 32 appelé\n");
    debug("In scheduler with index of current task : %d\n", current_task_index);

    task_t *task;

    task = &tasks[current_task_index];
    if (task->state == 0)
    {
        ctx->esp.raw = task->esp_user;
        ctx->eip.raw = task->eip;
        task->state = 1;
        task->ctx_task = *ctx;
    }
    else
    {
        tasks[(current_task_index + 1) % 2].ctx_task = *ctx;
        *ctx = task->ctx_task;
    }
    current_task_index = (current_task_index + 1) % 2;
    set_cr3(task->pgd);
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
    // init_interrup(32, 0, (offset_t)user_handler); L'interruption 32 est traité dans intr.c à cause de problèmes.
    init_interrup(80, 3, (offset_t)user_intr);
}