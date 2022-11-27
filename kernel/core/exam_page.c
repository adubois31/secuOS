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
   for (uint32_t i_pgd = 0; i_pgd < 3; i_pgd++)
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

   pte32_t *ptbs_kernel = (pte32_t *)0x800000; // taille PTBS = 0x100000
   pte32_t *ptbs_user1 = (pte32_t *)0x1200000;
   pte32_t *ptbs_user2 = (pte32_t *)0x1600000;
   init_pages(pgd_kernel, ptbs_kernel, PG_KRN);
   init_pages(pgd_user1, ptbs_user1, PG_USR);
   init_pages(pgd_user2, ptbs_user2, PG_USR);

   // display_pgd(pgd_user2);

   // uint32_t *v1 = (uint32_t *)0x548000;
   // uint32_t *test_shm = (uint32_t *)0x230000;

   // int page_index_shared_v1 = pt32_idx(v1);

   // debug("Page index : %d\n", page_index_shared_v1);
   // pg_set_entry(&ptbs_user1[page_index_shared_v1], PG_USR | PG_RW, (uint32_t)test_shm >> 12);

   // debug("ptbs à l'index page_index_shared : %x\n", ptbs_user1[page_index_shared_v1].addr);

   // *test_shm = 6;

   uint32_t *shared_test_mem = (uint32_t *)0x3;
   uint32_t *v1 = (uint32_t *)0x777000;

   int index = pt32_idx(v1);
   pg_set_entry(&ptbs_user1[index], PG_USR | PG_RW, (uint32_t)shared_test_mem);

   *shared_test_mem = 6;

   set_cr3(pgd_user1);
   enable_paging();

   debug("*sharedtestmem = %d, *v1 = %d\n", *shared_test_mem, *v1);

   //  // uint32_t *v2 = (uint32_t *)0x000200;
   //  set_cr3(pgd_user1);
   // debug("@test_shm : %p , valeur test_shm : %d\n", v1, *v1);
   // set_cr3(pgd_user2);
   // debug("@v2 : %p , valeur v2 : %d\n", v2, *v2);
}