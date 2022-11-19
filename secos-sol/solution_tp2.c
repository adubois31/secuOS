/* GPLv2 (c) Airbus */
#include <debug.h>
#include <segmem.h>
#include <info.h>

extern info_t *info;

// 2
void test_sti()
{
   while (1) asm volatile ("sti");
}

void bp_handler()
{
   asm volatile ("pusha");

   debug("#BP furtif\n");

   uint32_t eip;
   asm volatile ("mov 4(%%ebp), %0":"=r"(eip));
   debug("EIP = %p\n", eip);

   //asm volatile ("leave ; iret");

   asm volatile ("popa ; leave ; iret");
}

void bp_trigger()
{
   asm volatile ("int3");
   debug("after trigger bp\n");
}

void test_bp()
{
   idt_reg_t idtr;

   get_idtr(idtr);
   debug("IDT @ %p\n", idtr.addr);

   int_desc_t *bp_dsc = &idtr.desc[3];

   bp_dsc->offset_1 = (uint16_t)((uint32_t)bp_handler);
   bp_dsc->offset_2 = (uint16_t)(((uint32_t)bp_handler)>>16);

   bp_trigger();
}

void tp()
{
   test_sti();
   //test_bp();
}
