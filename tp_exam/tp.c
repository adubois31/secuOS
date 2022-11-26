/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>

#include <exam_segment.h>
#include <exam_page.h>

extern info_t *info;
extern uint32_t __kernel_start__;
extern uint32_t __kernel_end__;

void tp()
{

    debug("kernel mem [0x%x - 0x%x]\n", (uint32_t)&__kernel_start__, (uint32_t)&__kernel_end__);
    debug("MBI flags 0x%x\n", info->mbi->flags);

    // mbi_t *mbi;
    // multiboot_memory_map_t *start;
    // multiboot_memory_map_t *end;

    // mbi   = info->mbi;
    // start = (multiboot_memory_map_t*) mbi->mmap_addr;
    // end   = (multiboot_memory_map_t*)(mbi->mmap_addr + mbi->mmap_length);

    // while(start < end) {
    //     debug("mmap 0x%llx - 0x%llx (%d)\n",
    //             start->addr, start->addr+start->len, start->type);
    //     start++;
    // }
    debug("---Initialisation GDT---\n");
    init_gdt();
    debug("---Display GDT---\n");
    display_gdt();
    
    // debug("---Initialisation Pages---\n");
    // identity_init();
}
