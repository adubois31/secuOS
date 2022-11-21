#include <exam_segment.h>
#include <info.h>
#include <debug.h>

extern info_t *info;

void display_gdt(void)
{
  gdt_reg_t * gdtr = NULL;
  get_gdtr(*gdtr);

  int i = 0;
  seg_desc_t *begin;
  begin = (seg_desc_t *)gdtr->desc;
  while (begin <= (seg_desc_t *)gdtr->desc + gdtr->limit / sizeof(seg_desc_t))
  {
    uint32_t addr = (begin->base_1) | (begin->base_2 << 16) | (begin->base_3 << 24);
    uint32_t lim = (begin->limit_1) | (begin->limit_2 << 16);
    if (begin->g)
    {
      lim += 1;
      lim *= 4096;
      lim -= 1;
    }
    if (begin->p)
    {
      printf(
          "Segment %d : Adresse : [0x%x - 0x%x] : Type de descripteur: %d\n\n",
          i, addr, lim, begin->type);
    }
    begin++;
    i++;
  }
}

seg_desc_t create_dsc_flat(uint64_t type, uint64_t dpl)
{
  seg_desc_t seg;

  seg.limit_1 = 0xffff;
  seg.limit_2 = 0xf;

  seg.type = type; // segment type
  seg.dpl = dpl;   // descriptor privilege level (ring)

  seg.d = 1; // default length, depend on seg type
  seg.g = 1; // granularity
  seg.s = 1; // descriptor type
  seg.p = 1; // segment present flag

  return seg;
}

seg_desc_t create_dsc_tss(uint64_t base,uint64_t limit)
{
  seg_desc_t seg;
  
  seg.limit_1 = limit;
  seg.limit_2 = (limit >> 2 * 8);

  seg.base_1  = base;
  seg.base_2  = (base >> 2 * 8);
  seg.base_3  = (base >> 3 * 8);

  seg.type   = SEG_DESC_SYS_TSS_AVL_32;         
  seg.p      = 1; 

  return seg;
}

void init_gdt()
{
  gdt_reg_t gdtr;

  // Initialisation GDT et TSS
  seg_desc_t * GDT  = (seg_desc_t *)address_GDT;
  tss_t * TSS       = (tss_t*) address_TSS;

  // Segment vide rempli de 0, indice 0
  for (uint32_t i = 0; i < sizeof(seg_desc_t); i++)
  {
    *((uint8_t *)GDT + i) = 0;
  }

  // Kernel
  GDT[1] = create_dsc_flat(SEG_DESC_CODE_XR, 1);
  GDT[2] = create_dsc_flat(SEG_DESC_DATA_RW, 1);

  // User
  GDT[3] = create_dsc_flat(SEG_DESC_CODE_XR, 3);
  GDT[4] = create_dsc_flat(SEG_DESC_DATA_RW, 3);

  // TSS (pas sûr de moi)
  GDT[5] = create_dsc_tss(address_TSS, 0xfffff); // -------- SANS DOUTE A CHANGER
  memset((void*)address_TSS, 0, sizeof(tss_t));

  TSS->s0.esp = get_ebp();
  TSS->s0.ss  = d0_sel;
  set_tr(ts_sel);

  // Validation
  gdtr.desc  = GDT;
  gdtr.limit = 6 * sizeof(seg_desc_t) - 1; 
  set_gdtr(gdtr);

  set_cs(c0_sel); // -------- SANS DOUTE A CHANGER
  
  set_ss(d0_sel);
  set_ds(d0_sel);
  set_es(d0_sel);
  set_fs(d0_sel);
  set_gs(d0_sel);
}