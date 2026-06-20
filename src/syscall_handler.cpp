//
// Created by ugoboss on 6/12/26.
//

#include "../h/MemoryAllocator.hpp"

extern "C" void handle_trap() {

    unsigned code;
    asm volatile("csrr %0, a0" : "=r" (code));

    switch (code) {
        case 0x01:
            size_t size;
            asm volatile("csrr %0, a1" : "=r" (size));
            MemoryAllocator::mem_alloc(size);
            break;
        case 0x02:
            void* ptr;
            asm volatile("csrr %0, a1" : "=r" (ptr));
            MemoryAllocator::mem_free(ptr);
            break;
        //case 0x11:
            //...
        default:
            return;
    }
}
