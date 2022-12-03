#ifndef EXAM_SEGMENT
#define EXAM_SEGMENT
#include <string.h>
#include <segmem.h>

#define c0_idx 1
#define d0_idx 2
#define c3_idx 3
#define d3_idx 4
#define ts_idx 5

#define c0_sel gdt_krn_seg_sel(c0_idx)
#define d0_sel gdt_krn_seg_sel(d0_idx)
#define c3_sel gdt_usr_seg_sel(c3_idx)
#define d3_sel gdt_usr_seg_sel(d3_idx)
#define ts_sel gdt_krn_seg_sel(ts_idx)
// #define address_GDT 0x10000
// #define address_TSS 0x400000
extern tss_t *TSS;
extern seg_desc_t GDT[6];

void display_gdt(void);
void init_gdt();

#endif