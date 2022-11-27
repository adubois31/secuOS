#include <exam_interrup.h>
#include <segmem.h>
#include <intr.h>
#include <debug.h>

#include <exam_segment.h>

void init_kernel_interrup(){
    idt_reg_t idtr;
    get_idtr(idtr);

    int_desc_t * iface_dsc = &idtr.desc[0x80];
    int_desc(iface_dsc,c0_sel,(offset_t)kernel_handler);
    iface_dsc->type = SEG_DESC_SYS_TRAP_GATE_32;
    iface_dsc->dpl = 3;
}

void init_user_interrup(){
    idt_reg_t idtr;
    get_idtr(idtr);

    int_desc_t * iface_dsc = &idtr.desc[32];
    int_desc(iface_dsc,c0_sel,(offset_t)user_handler);
    iface_dsc->type = SEG_DESC_SYS_TRAP_GATE_32;
    iface_dsc->dpl = 0;
}


// Syscall pour afficher la valeur du compteur
__attribute__((naked)) void kernel_handler() {
    uint32_t counter;
    asm volatile (
        "mov %%eax, %0  \n" 
        "pusha          \n"
        : "=r"(counter));

    debug("Compteur = %d",counter);
    asm volatile ("popa; leave; iret");
}

// Syscall pour changer de task
__attribute__((naked)) void user_handler(){
    // AU secours
}
