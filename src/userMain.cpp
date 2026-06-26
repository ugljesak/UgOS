#include "../h/testMemoryAllocator.hpp"
#include "../h/testSystemThread.hpp"

void userMain(void* arg) {
    
    asm volatile("csrr a1, sstatus");

    testMemoryAllocator();
    printLine("Finished memory testing.");

    System_Mode_test();
    printLine("Finished threads testing.");

}