#include "../lib/console.h"
#include "../h/syscall_c.hpp"
#include "../h/utils.hpp"
#include "../h/MemoryAllocator.hpp"

void testMemoryAllocator() {

    printReg("scause", 1);
    printReg("sepc", 1);
    printReg("stval", 1);

    printLine("Start");
    void* a=mem_alloc(64);
    if(a) {
        printLine("Allocated a.");
    }
    void* b=mem_alloc(64);
    if(b) {
        printLine("Allocated b.");
    }
    mem_free(a);
    printLine("Freed a");
    void* p=mem_alloc(64);
    if (p==a) {
        printLine("Recycled p to a succesfully.");
    }
    mem_free(b);
    mem_free(p);
    printLine("Freed b and p.");
}