#include "../lib/hw.h"
#include "../h/TCB.hpp"
#include "../h/Scheduler.hpp"
#include "../h/utils.hpp"
#include "../h/testMemoryAllocator.hpp"
#include "../h/testSystemThread.hpp"

void trap_entry(); 
extern void userMain(void*);

int main() {
    MemoryAllocator::init();
    asm volatile("csrw stvec, %0" : : "r"(&trap_entry));
    TCB::init();

    TCB* user = TCB::createUserThread(userMain, nullptr);
    // testMemoryAllocator();
    // printLine("Finished memory testing.");

    // System_Mode_test();
    // printLine("Finished threads testing.");
    printLine("Stigao.");


    testMemoryAllocator();
    printLine("Finished memory testing.");

    System_Mode_test();
    printLine("Finished threads testing.");

    while(!user->isFinished()) {
        TCB::dispatch();
    }

    asm volatile(
        "li t0, 0x100000\n"
        "li t1, 0x5555\n"
        "sw t1, 0(t0)\n"
    );
    return 0;
}