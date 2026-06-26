#include "../h/utils.hpp"


void userMain(void* arg) {
    
    printLine("kurcina.");
    asm volatile("csrr a1, sstatus");


    //testMemoryAllocator();
    printLine("Finished memory testing.");

    //System_Mode_test();
    printLine("Finished threads testing.");

}