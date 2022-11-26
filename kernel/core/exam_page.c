#include <exam_page.h>
#include <debug.h>

extern info_t *info;

void display_pte(pte32_t *pte, uint32_t offset)
{
   for (uint32_t i = 0; i < 3; i++)
   {
      if (pte[i].addr > 0)
      {
         printf("-- (%d) : Virtuelle : 0x%x -> Physique : 0x%x\n", i, (i | (offset << 10)) << 12, pte[i].addr << 12);
      }
   }
}

void display_pgd(pde32_t *pgd)
{
   debug("---Display PGD---\n");
   printf("Address PGD : %p\n", (void *)pgd);

   for (uint32_t i = 0; i < PDE32_PER_PD; i++)
   {
      if (pgd[i].addr > 0)
      {
         printf("- Index : %d Address PTB : 0x%x\n", i, pgd[i].addr << 12);
         display_pte((pte32_t *)(pgd[i].addr << 12), i);
      }
   }
}

void enable_paging()
{
   uint32_t cr0 = get_cr0();
   set_cr0(cr0 | CR0_PG);
}

void init_pages(pde32_t *pgd, pte32_t *ptb, int lvl)
{
   memset((void *)pgd, 0, PAGE_SIZE);
   for (uint32_t i_pgd = 0; i_pgd < PDE32_PER_PD; i_pgd++)
   {
      pg_set_entry(&pgd[i_pgd], lvl | PG_RW, page_nr(&ptb[i_pgd * PTE32_PER_PT]));
      for (uint32_t i_ptb = 0; i_ptb < PTE32_PER_PT; i_ptb++)
      {
         pg_set_entry(&ptb[i_ptb + i_pgd * PTE32_PER_PT], lvl | PG_RW, i_ptb + i_pgd * 1024);
      }
   }
}

void identity_init()
{

   pde32_t *pgd_kernel = (pde32_t *)0x600000; // taille PGD  = 0x400
   pde32_t *pgd_user1 = (pde32_t *)0x601000;
   pde32_t *pgd_user2 = (pde32_t *)0x602000;

   pte32_t *ptbs_kernel = (pte32_t *)0xf00000; // taille PTBS = 0x100000
   pte32_t *ptbs_user1 = (pte32_t *)0xb00000;
   pte32_t *ptbs_user2 = (pte32_t *)0x900000;

   init_pages(pgd_kernel, ptbs_kernel, PG_KRN);
   init_pages(pgd_user1, ptbs_user1, PG_USR);
   init_pages(pgd_user2, ptbs_user2, PG_USR);

   set_cr3(pgd_kernel);
   enable_paging();
   display_pgd(pgd_user2);

   pg_set_entry();

   // ----------

   // debug("PTB[1] = %p\n", (void*)ptbs_user1[0][0].raw);

   // pte32_t  *ptb3    = (pte32_t*)0x603000;
   // uint32_t *target  = (uint32_t*)0xc0000000;
   // int      pgd_idx = pd32_idx(target);
   // int      ptb_idx = pt32_idx(target);

   // memset((void*)ptb3, 0, PAGE_SIZE);
   // pg_set_entry(&ptb3[ptb_idx], PG_KRN|PG_RW, page_nr(pgd));
   // pg_set_entry(&pgd[pgd_idx], PG_KRN|PG_RW, page_nr(ptb3));

   // debug("PGD[0] = %p | target = %p\n", (void*)pgd[0].raw, (void*)*target);

   // char *v1 = (char*)0x700000;
   // char *v2 = (char*)0x7ff000;

   // ptb_idx = pt32_idx(v1);
   // pg_set_entry(&ptb2[ptb_idx], PG_KRN|PG_RW, 2);

   // ptb_idx = pt32_idx(v2);
   // pg_set_entry(&ptb2[ptb_idx], PG_KRN|PG_RW, 2);

   // debug("%p = %s | %p = %s\n", (void*)v1, v1, (void*)v2, v2);

   // *target = 0;
   // display_pgd();
}