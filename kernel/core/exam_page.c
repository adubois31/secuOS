#include <exam_page.h>
#include <debug.h>

extern info_t *info;

void display_pte(pte32_t *pte, uint32_t offset)
{
   for (uint32_t i = 0; i < 3; i++)
   {
      if (pte[i].addr > 0)
      {
         printf("-- (%d) : Virtuelle : %p -> Physique : %p\n", i, (void *)((i | (offset << 10)) << 12), (void *)(pte[i].addr << 12));
      }
   }
}

void display_pgd(pde32_t *pgd)
{
   printf("---Display PGD %p --- \n", (void *)pgd);

   for (uint32_t i = 0; i < PDE32_PER_PD; i++)
   {
      if (pgd[i].addr > 0)
      {
         printf("- Index : %d Address PTB : %p\n", i, (void *)(pgd[i].addr << 12));
         display_pte((pte32_t *)(pgd[i].addr << 12), i);
      }
   }
}

void enable_paging()
{
   uint32_t cr0 = get_cr0();
   set_cr0(cr0 | CR0_PG);
}

pte32_t *shiftPTB(pte32_t *ptb, int offsetPTB)
{
   return (pte32_t *)((void *)ptb + sizePTB * offsetPTB);
}

void init_pgd(pde32_t *pgd, pte32_t *ptb, int lvl)
{
   for (uint32_t i_pgd = 0; i_pgd < 6; i_pgd++)
   {
      pg_set_entry(&pgd[i_pgd], lvl | PG_RW, (int)shiftPTB(ptb, i_pgd) >> 12);
   }
}

void init_ptb(pte32_t *ptb, int offsetPTB, int offsetPhy, int lvl)
{
   for (int i = 0; i < 1024; i++)
   {
      pg_set_entry(&shiftPTB(ptb, offsetPTB)[i], (PG_RW | lvl), i + 1024 * offsetPhy);
   }
}

void page_init()
{

   pde32_t *pgd_kernel = address_PGD_kernel;
   pde32_t *pgd_user1 = address_PGD_usr1;
   pde32_t *pgd_user2 = address_PGD_usr2;

   pte32_t *ptbs_kernel = addresss_PTBS_kernel;
   pte32_t *ptbs_user1 = addresss_PTBS_usr1;
   pte32_t *ptbs_user2 = addresss_PTBS_usr2;

   int shm_ptb_idx;

   init_pgd(pgd_kernel, ptbs_kernel, PG_KRN);
   init_ptb(ptbs_kernel, 0, 0, PG_KRN); // Kernell stack ()
   init_ptb(ptbs_kernel, 1, 1, PG_KRN); // De la place pour la shm entre autre

   init_pgd(pgd_user1, ptbs_user1, PG_USR);
   init_ptb(ptbs_user1, 0, 0, PG_USR);    // Kernell stack
   init_ptb(ptbs_user1, 2, 2, PG_USR);    // User1   stack
   shm_ptb_idx = pt32_idx(shm_vir_user1); // SHM
   pg_set_entry(&shiftPTB(ptbs_user1, 2)[shm_ptb_idx], (PG_RW | PG_USR), (int)(shm_phy) >> 12);

   init_pgd(pgd_user2, ptbs_user2, PG_USR);
   init_ptb(ptbs_user2, 0, 0, PG_USR);    // Kernell stack
   init_ptb(ptbs_user2, 3, 3, PG_USR);    // User2   stack
   shm_ptb_idx = pt32_idx(shm_vir_user2); // SHM
   pg_set_entry(&shiftPTB(ptbs_user2, 3)[shm_ptb_idx], (PG_RW | PG_USR), (int)(shm_phy) >> 12);

   display_pgd(pgd_user1);

   *((uint32_t *)shm_phy) = 4;

   set_cr3(pgd_user1);
   enable_paging();
   debug("@v1 : %x , valeur : %d\n", shm_vir_user1, *((uint32_t *)shm_vir_user1));
}