/* GPLv2 (c) Airbus */
#include <exam_segment.h>
#include <debug.h>
#include <info.h>

tss_t *TSS = (tss_t *)address_TSS;
seg_desc_t GDT[6];

extern info_t *info;

#define gdt_flat_dsc(_dSc_, _pVl_, _tYp_) \
   ({                                     \
      (_dSc_)->raw = 0;                   \
      (_dSc_)->limit_1 = 0xffff;          \
      (_dSc_)->limit_2 = 0xf;             \
      (_dSc_)->type = _tYp_;              \
      (_dSc_)->dpl = _pVl_;               \
      (_dSc_)->d = 1;                     \
      (_dSc_)->g = 1;                     \
      (_dSc_)->s = 1;                     \
      (_dSc_)->p = 1;                     \
   })

#define tss_dsc(_dSc_, _tSs_)                  \
   ({                                          \
      raw32_t addr = {.raw = _tSs_};           \
      (_dSc_)->raw = sizeof(tss_t);            \
      (_dSc_)->base_1 = addr.wlow;             \
      (_dSc_)->base_2 = addr._whigh.blow;      \
      (_dSc_)->base_3 = addr._whigh.bhigh;     \
      (_dSc_)->type = SEG_DESC_SYS_TSS_AVL_32; \
      (_dSc_)->p = 1;                          \
   })

#define c0_dsc(_d) gdt_flat_dsc(_d, 0, SEG_DESC_CODE_XR)
#define d0_dsc(_d) gdt_flat_dsc(_d, 0, SEG_DESC_DATA_RW)
#define c3_dsc(_d) gdt_flat_dsc(_d, 3, SEG_DESC_CODE_XR)
#define d3_dsc(_d) gdt_flat_dsc(_d, 3, SEG_DESC_DATA_RW)

void display_gdt(void)
{
   gdt_reg_t gdtr;
   get_gdtr(gdtr);

   int i = 0;
   seg_desc_t *begin;
   begin = (seg_desc_t *)gdtr.desc;
   while (begin <= (seg_desc_t *)gdtr.desc + gdtr.limit / sizeof(seg_desc_t))
   {
      uint32_t addr = (begin->base_1) | (begin->base_2 << 16) | (begin->base_3 << 24);
      uint32_t lim = (begin->limit_1) | (begin->limit_2 << 16);
      if (begin->g)
      {
         lim += 1;
         lim *= 4096;
         lim -= 1;
      }
      debug(
          "Segment %d : Adresse : [0x%x - 0x%x] : Type de descripteur: %d\n",
          i, addr, lim, begin->type);
      begin++;
      i++;
   }
}

// Faire une fonction ici pour modif TSS (quand on change de task ?)

void init_gdt()
{
   gdt_reg_t gdtr;
   GDT[0].raw = 0ULL;

   c0_dsc(&GDT[c0_idx]);
   d0_dsc(&GDT[d0_idx]);
   c3_dsc(&GDT[c3_idx]);
   d3_dsc(&GDT[d3_idx]);

   gdtr.desc = GDT;
   gdtr.limit = sizeof(GDT) - 1;
   set_gdtr(gdtr);

   set_cs(c0_sel);

   set_ss(d0_sel);
   set_ds(d0_sel);
   set_es(d0_sel);
   set_fs(d0_sel);
   set_gs(d0_sel);

   TSS->s0.esp = get_ebp();
   TSS->s0.ss = d0_sel;
   tss_dsc(&GDT[ts_idx], (offset_t)&TSS);
   set_tr(ts_sel);
}