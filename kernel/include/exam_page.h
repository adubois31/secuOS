#ifndef EXAM_PAGE
#define EXAM_PAGE

#include <cr.h>
#include <pagemem.h>
#include <info.h>

void display_pte(pte32_t *pte, uint32_t offset);
void display_pgd();
void enable_paging();
void page_init();

#endif