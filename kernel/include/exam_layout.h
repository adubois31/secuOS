#ifndef EXAM_LAYOUT
#define EXAM_LAYOUT
#include <segmem.h>

#define sizePTB 0x1000

tss_t *TSS;
seg_desc_t GDT[6];

#define address_PGD_kernel (pde32_t *)0x100000 // taille PGD  = 0x400
#define address_PGD_usr1 (pde32_t *)0x101000
#define address_PGD_usr2 (pde32_t *)0x102000

#define addresss_PTBS_kernel (pte32_t *)0x200000 // taille 1 ptb = 0x1000 or on en a 2 pour chaque
#define addresss_PTBS_usr1 (pte32_t *)0x210000
#define addresss_PTBS_usr2 (pte32_t *)0x220000

#define STACK_KERNEL_USER1 0x405000
#define STACK_KERNEL_USER2 0x505000

#define STACK_USER1 0x800000
#define STACK_USER2 0xc00000

#define shm_phy 0x600000
#define shm_vir_user1 0x805000 // on décale un peu du début car y'a les fonctions qui sont stockés
#define shm_vir_user2 0xc05000

#endif