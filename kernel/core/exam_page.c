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

   pde32_t *pgd_kernel = (pde32_t *)address_PGD_kernel;
   pde32_t *pgd_user1 = (pde32_t *)address_PGD_usr1;
   pde32_t *pgd_user2 = (pde32_t *)address_PGD_usr2;

   pte32_t *ptbs_kernel = (pte32_t *)0xf00000; // taille PTBS = 0x100000
   pte32_t *ptbs_user1 = (pte32_t *)0xb00000;
   pte32_t *ptbs_user2 = (pte32_t *)0x900000;

   init_pages(pgd_kernel, ptbs_kernel, PG_KRN);
   init_pages(pgd_user1, ptbs_user1, PG_USR);
   init_pages(pgd_user2, ptbs_user2, PG_USR);

   set_cr3(pgd_kernel);
   enable_paging();

   pg_set_entry(pgd_user1, PG_KRN | PG_RW, 0xfff);

   int pgd_index = pd32_idx(shared_mem);
   int ptb_index = pt32_idx(shared_mem);

   // Adress, RW,
   pg_set_entry(&ptbs_user1[1023 * 1024 + 1021], PG_KRN | PG_RW, 1021 + (1023 << 10));
   pg_set_entry(&ptbs_user2[1023 * 1024 + 1023], PG_KRN | PG_RW, 1023 + (1023 << 10));

   debug("PGDuser1[0] = %p | target = %p\n", (void *)pgd_user1[0].raw, (void *)*shared_mem);
   debug("PGDuser2[0] = %p | target = %p\n", (void *)pgd_user2[0].raw, (void *)*shared_mem);

   *shared_mem = 0;
   debug("%p = %d | %p = %d\n", (void *)v1, *v1, (void *)v2, *v2);
   *shared_mem = 3;
   debug("%p = %d | %p = %d\n", (void *)v1, *v1, (void *)v2, *v2);
}