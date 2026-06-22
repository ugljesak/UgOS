#include "../lib/hw.h"
#include "../h/testSyscall_C.hpp"
#include "../h/MemoryAllocator.hpp"

extern "C" void trap_entry(); 
extern void memoryAllocatorClassTest(void);

int main() {
    MemoryAllocator::init();
    memoryAllocatorClassTest();
    asm volatile("csrw stvec, %0" : : "r"(&trap_entry));

    testMemoryAllocator();

    return 0;
}