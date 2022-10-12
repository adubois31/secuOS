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

   // Lecture à un emplacement mémoire
   int * pmem = (int*) 0x9fc00;
   debug("Valeur %d\n", *pmem);
   // Marche bien

   // Ecriture à l'emplacement
   *pmem = 3;
   debug("Valeur %d\n", *pmem);
   // Erreur donc readOnly

   // RAM = 128Mb = 0x80, on écrit un peu après à 0x90
   int * pmem = (int*) 0x300090;
   debug("Valeur %d\n", *pmem);
   // Lecture OK
   *pmem = 3;
   debug("Valeur %d\n", *pmem);
   // Ecriture OK

}
