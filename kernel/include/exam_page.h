#ifndef EXAM_PAGE
#define EXAM_PAGE

#include <cr.h>
#include <pagemem.h>
#include <info.h>

// #define address_PGD 0x600000
// #define address_PTB 0x601000;

void display_pte(pte32_t *pte, uint32_t offset);
void display_pgd();
void enable_paging();
void init_pages(pde32_t* pgd, pte32_t* ptb, int lvl);
void identity_init();

#endif