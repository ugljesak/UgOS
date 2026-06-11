//
// Created by ugoboss on 6/10/26.
//

#include "../h/MemoryAllocator.hpp"


MemoryAllocator::FreeBlockHeader* MemoryAllocator::freeMemHead = nullptr;

void* MemoryAllocator::mem_alloc(size_t size) {
    //FreeBlockHeader* curr = freeMemHead;
    //FreeBlockHeader* prev = nullptr;
    return nullptr;
}

int MemoryAllocator::mem_free(void *ptr) {

    return 0;
}
