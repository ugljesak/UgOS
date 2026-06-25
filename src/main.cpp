#include "../lib/hw.h"
#include "../h/testMemoryAllocator.hpp"
#include "../h/testSystemThread.hpp"
#include "../h/TCB.hpp"
#include "../h/Scheduler.hpp"

void trap_entry(); 

int main() {
    MemoryAllocator::init();
    asm volatile("csrw stvec, %0" : : "r"(&trap_entry));

    TCB* main = (TCB*)MemoryAllocator::mem_alloc(sizeof(TCB));
    main->kernelStack = MemoryAllocator::mem_alloc(64 * DEFAULT_STACK_SIZE);
    main->sp  = (uint64)((char*)main->kernelStack + 64 * DEFAULT_STACK_SIZE - 104);

    asm volatile("csrw sscratch, %0" : : "r"(main->sp));

    TCB::running = main;

    //testMemoryAllocator();
    //printLine("Finished memory testing.");

    System_Mode_test();
    printLine("Finished threads testing.");
    
    asm volatile(
        "li t0, 0x100000\n"
        "li t1, 0x5555\n"
        "sw t1, 0(t0)\n"
    );
    return 0;
}