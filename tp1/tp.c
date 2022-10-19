/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>
#include <segmem.h>
#include <string.h>


extern info_t *info;

void tp(){
    gdt_reg_t * gdt;
    get_gdtr(gdt);
    debug("addr : %x\n",gdt->desc->type);
    debug("addr2 : %lx\n",gdt->addr);

    //seg_desc_t desc1;
    //desc1.dpl = 10;

    //seg_desc_t desc2;
    //desc2.dpl = 2;

    //(gdt + sizeof(desc1)) = &desc1 ;
}
