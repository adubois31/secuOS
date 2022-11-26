#ifndef EXAM_PAGE
#define EXAM_PAGE

#include <cr.h>
#include <pagemem.h>
#include <info.h>

#define address_PGD_kernel 0x600000 // taille PGD  = 0x400
#define address_PGD_usr1 0x601000
#define address_PGD_usr2 0x602000
#define shared_mem (uint32_t *)0xf0696969

void display_pte(pte32_t *pte, uint32_t offset);
void display_pgd();
void enable_paging();
void init_pages(pde32_t *pgd, pte32_t *ptb, int lvl);
void identity_init();

#endif