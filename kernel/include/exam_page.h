#ifndef EXAM_PAGE
#define EXAM_PAGE

#include <cr.h>
#include <pagemem.h>
#include <info.h>

// #define address_PGD 0x600000
// #define address_PTB 0x601000;

void show_cr3();
void enable_paging();
void identity_init();

#endif