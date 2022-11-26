#include <exam_task.h>
#include <exam_page.h>
#include <asm.h>

// On a choisi de mettre counter au début de la shm
// Mais on peut le modifier

uint32_t* counter = (uint32_t *)0x10000;

void sys_counter(uint32_t* counter){
    asm volatile (
        "mov %0, %%eax  \n" 
        "int $0x80      \n"
        :: "r"(counter));
}


// Incrémente 
__attribute__((section(".user"))) void user1(){
    while (1){
        (*counter)++;
    } 
}

// Display
__attribute__((section(".user"))) void user2(){
    while (1){   
        sys_counter(counter);
    }
}

void init_tasks(){
    (*counter) = 0;
}