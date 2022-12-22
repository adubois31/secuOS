#include <exam_interrup.h>
#include <exam_segment.h>
#include <exam_task.h>
#include <exam_layout.h>
#include <debug.h>

extern int current_task_index;
extern task_t tasks[NB_TASKS];

void init_user1(int_ctx_t * esp){
   esp->eip.raw            = (uint32_t) &user1;
   esp->cs.raw             = c3_sel;
   esp->eflags.raw         = EFLAGS_IF;
   esp->esp.raw            = stack_user1;
   esp->ss.raw             = d3_sel;
   esp->gpr.ebp.raw        = stack_user1;
}
void init_user2(int_ctx_t * esp){
   esp->eip.raw            = (uint32_t) &user2;
   esp->cs.raw             = c3_sel;
   esp->eflags.raw         = EFLAGS_IF;
   esp->esp.raw            = stack_user2;
   esp->ss.raw             = d3_sel;
   esp->gpr.ebp.raw        = stack_user1;
}

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
    // debug("%x\n", ctx->gpr.eax.raw);
    ctx = ctx;
    asm volatile("popa; iret");
}

// Syscall pour changer de task
__attribute__((naked)) __regparm__(1) void user_handler(int_ctx_t *ctx)
{
    debug("Changement de tâche, précédente : %d\n", current_task_index);
    task_t *task;

    if (current_task_index == -1)
    {
        current_task_index = 0;
        init_user1((int_ctx_t *) tasks[0].esp_kernel);
        init_user2((int_ctx_t *) tasks[1].esp_kernel);
        set_ds(d3_sel);
        set_es(d3_sel);
        set_fs(d3_sel);
        set_gs(d3_sel);
    }
    else
    {
        // Sauvegarder contexte ?
        tasks[current_task_index].esp_kernel = (uint32_t)ctx;
        asm volatile("mov %%esp, %0"
                    : "=r"(tasks[current_task_index].esp_kernel));

        current_task_index = (current_task_index + 1) % 2;
        asm volatile("mov %0, %%esp" 
                    ::"r"(tasks[current_task_index].esp_kernel));
    }

    task = &tasks[current_task_index];
    set_esp(task->esp_kernel);
    tss_t *TSS = (tss_t *)address_TSS;
    TSS->s0.esp = task->esp_kernel;
    // TSS->s0.ss = gdt_krn_seg_sel(1);
    // TSS->eip = task->eip;
    set_cr3(task->pgd);
    asm volatile("popa ; add $8, %esp ; iret");
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