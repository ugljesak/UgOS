#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"

void memoryAllocatorClassTest() {
    __putc('S');
    void* a=MemoryAllocator::mem_alloc(64);
    if(a) {
        __putc('1');
    }
    void* b=MemoryAllocator::mem_alloc(64);
    if(b) {
        __putc('2');
    }
    MemoryAllocator::mem_free(a);
    __putc('F');
    void* p=MemoryAllocator::mem_alloc(64);
    if (p==a) {
        __putc('M');
    }
    __putc('E');
    MemoryAllocator::mem_free(b);
    MemoryAllocator::mem_free(p);
    __putc('\n');
}