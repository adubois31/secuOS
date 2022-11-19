/* GPLv2 (c) Airbus */
#include <debug.h>
#include <segmem.h>
#include <string.h>
#include <info.h>

extern info_t *info;

#define c0_idx  1
#define d0_idx  2

#define c0_sel  gdt_krn_seg_sel(c0_idx)
#define d0_sel  gdt_krn_seg_sel(d0_idx)

seg_desc_t GDT[6];

#define gdt_flat_dsc(_dSc_,_pVl_,_tYp_)                                 \
   ({                                                                   \
      (_dSc_)->raw     = 0;                                             \
      (_dSc_)->limit_1 = 0xffff;                                        \
      (_dSc_)->limit_2 = 0xf;                                           \
      (_dSc_)->type    = _tYp_;                                         \
      (_dSc_)->dpl     = _pVl_;                                         \
      (_dSc_)->d       = 1;                                             \
      (_dSc_)->g       = 1;                                             \
      (_dSc_)->s       = 1;                                             \
      (_dSc_)->p       = 1;                                             \
   })

#define c0_dsc(_d) gdt_flat_dsc(_d,0,SEG_DESC_CODE_XR)
#define d0_dsc(_d) gdt_flat_dsc(_d,0,SEG_DESC_DATA_RW)

void show_gdt()
{
   gdt_reg_t gdtr;
   size_t    i,n;

   get_gdtr(gdtr);
   n = (gdtr.limit+1)/sizeof(seg_desc_t);
   for(i=0 ; i<n ; i++) {
      seg_desc_t *dsc   = &gdtr.desc[i];
      uint32_t    base  = dsc->base_3<<24 | dsc->base_2<<16 | dsc->base_1;
      uint32_t    limit = dsc->limit_2<<16| dsc->limit_1;
      debug("GDT[%d] = 0x%llx | base 0x%x | limit 0x%x | type 0x%x\n",
            i, gdtr.desc[i].raw, base, limit, dsc->type);
   }
   debug("--\n");
}

void init_gdt()
{
   gdt_reg_t gdtr;

   GDT[0].raw = 0ULL;

   c0_dsc( &GDT[c0_idx] );
   d0_dsc( &GDT[d0_idx] );

   gdtr.desc  = GDT;
   gdtr.limit = sizeof(GDT) - 1;
   set_gdtr(gdtr);

   set_cs(c0_sel);

   set_ss(d0_sel);
   set_ds(d0_sel);
   set_es(d0_sel);
   set_fs(d0_sel);
   set_gs(d0_sel);
}

void overflow()
{
   uint32_t base = 0x600000;

   GDT[3].raw = 0;

   GDT[3].base_1 = base;
   GDT[3].base_2 = base >> 16;
   GDT[3].base_3 = base >> 24;

   GDT[3].limit_1 = 32 - 1;

   GDT[3].type = SEG_DESC_DATA_RW;
   GDT[3].d   = 1;
   GDT[3].s   = 1;
   GDT[3].p   = 1;
   GDT[3].dpl = 0;

   char  src[64];
   char *dst = 0;
   memset(src, 0xff, 64);

   set_es(gdt_krn_seg_sel(3));

   //_memcpy8(dst, src, 32);
   _memcpy8(dst, src, 64);
}

void tp()
{
   show_gdt();
   init_gdt();
   overflow();
}
