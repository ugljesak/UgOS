//
// Created by ugoboss on 6/10/26.
//

#ifndef UGOS_MEMORY_ALLOCATOR_H
#define UGOS_MEMORY_ALLOCATOR_H

#include "../lib/hw.h"

class MemoryAllocator {
public:
    static void* mem_alloc(size_t size);
    static int mem_free(void* ptr);

private:
    struct FreeBlockHeader {
        size_t size;
        FreeBlockHeader* next;
    };
    static FreeBlockHeader* freeMemHead;
};

#endif  // UGOS_MEMORY_ALLOCATOR_H
