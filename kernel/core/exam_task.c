#include <exam_task.h>
#include <exam_page.h>
#include <asm.h>

__attribute__((section(".user1"))) void user1(){
    while (true){
    } 
}

__attribute__((section(".user2"))) void user2(){
    while (true){
        
    }
}

void init_tasks(){

}