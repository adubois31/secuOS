/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>

extern info_t   *info;
extern uint32_t __kernel_start__;
extern uint32_t __kernel_end__;

void tp() {
   debug("kernel mem [0x%p - 0x%p]\n", &__kernel_start__, &__kernel_end__);
   debug("MBI flags 0x%x\n", info->mbi->flags);

   mbi_t * mbi = info->mbi;
   multiboot_memory_map_t * start = (multiboot_memory_map_t *)mbi->mmap_addr;
   multiboot_memory_map_t * end   = (multiboot_memory_map_t *)(mbi->mmap_addr + mbi->mmap_length);

   while(start < end){
      debug("mmap 0x%llx - 0x%llx (%d)\n",
         start->addr, start->addr + start->len, start->type);
      start ++;
   }

   // Lecture à un emplacement mémoire valide: et qui marche bien
   int * pmem = (int*) 0x9fc00;
   debug("Valeur %d\n", *pmem);
   
   *pmem = 3;
   debug("Valeur %d\n", *pmem);

   // RAM = 128Mb = 0x80, on écrit un peu après à 0x90
   pmem = (int*) 0x8000000;
   debug("Valeur %d\n", *pmem);
   // Lecture: pas d'erreur
   *pmem = 3;
   debug("Valeur %d\n", *pmem);
   // On retourne 0 donc on voit qu'on ne peut pas écrire

}
