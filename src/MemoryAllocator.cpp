//
// Created by ugoboss on 6/10/26.
//

#include "../h/MemoryAllocator.hpp"

MemoryAllocator::FreeBlockHeader* MemoryAllocator::freeMemHead = nullptr;

void MemoryAllocator::init() {
    freeMemHead = (FreeBlockHeader*)HEAP_START_ADDR;
    size_t totalSize = (char*)HEAP_END_ADDR - (char*)HEAP_START_ADDR;
    freeMemHead->size = totalSize - sizeof(FreeBlockHeader);
    freeMemHead->next = nullptr;
}

void* MemoryAllocator::mem_alloc(size_t size) {

    if (freeMemHead == nullptr) {
        init();
    }

    FreeBlockHeader* curr = freeMemHead;
    FreeBlockHeader* prev = nullptr;

    size_t alignedSize = ((size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE) * MEM_BLOCK_SIZE;

    for (; curr != nullptr; prev = curr, curr = curr->next) {
        if (curr->size >= alignedSize) {
            break;
        }
    }

    if (curr == nullptr) {
        return nullptr;
    }

    size_t remSize = curr->size - alignedSize;
    if (remSize >= sizeof(FreeBlockHeader) + MEM_BLOCK_SIZE) {
        size_t offset = sizeof(FreeBlockHeader) + alignedSize;
        curr->size = alignedSize;

        FreeBlockHeader* newBlock = (FreeBlockHeader*)((char*)curr + offset);
        if (prev != nullptr) {
            prev->next = newBlock;
        }
        else {
            freeMemHead = newBlock;
        }
        newBlock->next = curr->next;
        newBlock->size = remSize - sizeof(FreeBlockHeader);
    }
    else {
        if (prev) {
            prev->next = curr->next;
        }
        else {
            freeMemHead = curr->next;
        }
    }
    curr->next = nullptr;
    return (char*)curr + sizeof(FreeBlockHeader);
}

void MemoryAllocator::tryCoalesce(FreeBlockHeader* ptr) {
    if (ptr == nullptr || ptr->next == nullptr) {
        return;
    }
    if ((char*)ptr + sizeof(FreeBlockHeader) + ptr->size == (char*)ptr->next) {
        ptr->size += sizeof(FreeBlockHeader) + ptr->next->size;
        ptr->next = ptr->next->next;
    }
}

int MemoryAllocator::mem_free(void *ptr) {
    if (ptr == nullptr) {
        return -1;
    }

    FreeBlockHeader* target = (FreeBlockHeader*)((char*)ptr - sizeof(FreeBlockHeader));

    FreeBlockHeader* curr = freeMemHead;
    FreeBlockHeader* prev = nullptr;

    for (; curr != nullptr; prev = curr, curr = curr->next) {
        if (curr > target) {
            break;
        }
    }

    if (prev != nullptr) {
        prev->next = target;
    }
    else {
        freeMemHead = target;
    }
    target->next = curr;

    tryCoalesce(curr);
    tryCoalesce(prev);

    return 0;
}
