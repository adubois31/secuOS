#ifndef EXAM_SEGMENT
#define EXAM_SEGMENT

#include <segmem.h>
#include <string.h>

#define c0_sel  gdt_krn_seg_sel(1)
#define d0_sel  gdt_krn_seg_sel(2)
#define c3_sel  gdt_usr_seg_sel(3)
#define d3_sel  gdt_usr_seg_sel(4)
#define ts_sel  gdt_krn_seg_sel(5)

#define address_GDT 0x10000
#define address_TSS 0x400000

void display_gdt(gdt_reg_t *gdtr);
seg_desc_t create_dsc_flat(uint64_t type, uint64_t dpl);
seg_desc_t create_dsc_tss(uint64_t base,uint64_t limit);
void init_gdt();

#endif