/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>
#include <segmem.h>


extern info_t *info;

void tp(){
    gdt_reg_t * gdt;
    get_gdtr(gdt);
}
