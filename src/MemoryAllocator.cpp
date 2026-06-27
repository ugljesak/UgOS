#include "../h/MemoryAllocator.hpp"
#include "../h/utils.hpp"


MemoryAllocator::FreeBlockHeader* MemoryAllocator::freeMemHead = nullptr;

void MemoryAllocator::init() {
    freeMemHead = (FreeBlockHeader*)HEAP_START_ADDR;
    size_t totalSize = (char*)HEAP_END_ADDR - (char*)HEAP_START_ADDR;
    freeMemHead->size = totalSize - sizeof(FreeBlockHeader);
    freeMemHead->next = nullptr;
}

void* MemoryAllocator::mem_alloc(size_t size) {
    
    size_t headerSize = (sizeof(FreeBlockHeader) + MEM_BLOCK_SIZE - 1)/MEM_BLOCK_SIZE*MEM_BLOCK_SIZE;

    FreeBlockHeader* curr = freeMemHead;
    FreeBlockHeader* prev = nullptr;

    for (; curr != nullptr; prev = curr, curr = curr->next) {
        if (curr->size >= size) {
            break;
        }
    }

    if (curr == nullptr) {
        return nullptr;
    }

    size_t remSize = curr->size - size;
    if (remSize >= headerSize + MEM_BLOCK_SIZE) {
        size_t offset = headerSize + size;
        curr->size = size;

        FreeBlockHeader* newBlock = (FreeBlockHeader*)((char*)curr + offset);
        if (prev != nullptr) {
            prev->next = newBlock;
        }
        else {
            freeMemHead = newBlock;
        }
        newBlock->next = curr->next;
        newBlock->size = remSize - headerSize;
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
    return (char*)curr + headerSize;
}

void MemoryAllocator::tryMerge(FreeBlockHeader* ptr) {
    size_t headerSize = (sizeof(FreeBlockHeader) + MEM_BLOCK_SIZE - 1)/MEM_BLOCK_SIZE*MEM_BLOCK_SIZE;

    if (ptr == nullptr || ptr->next == nullptr) {
        return;
    }
    if ((char*)ptr + headerSize + ptr->size == (char*)ptr->next) {
        ptr->size += headerSize + ptr->next->size;
        ptr->next = ptr->next->next;
    }
}

int MemoryAllocator::mem_free(void *ptr) {
    if (ptr == nullptr) {
        return -1;
    }
    size_t headerSize = (sizeof(FreeBlockHeader) + MEM_BLOCK_SIZE - 1)/MEM_BLOCK_SIZE*MEM_BLOCK_SIZE;

    FreeBlockHeader* target = (FreeBlockHeader*)((char*)ptr - headerSize);

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

    tryMerge(target);
    tryMerge(prev);

    return 0;
}

void MemoryAllocator::printList() {
    printLine("--- Free List ---");
    FreeBlockHeader* curr = freeMemHead;
    int i = 0;
    while (curr != nullptr) {
        printString("[");
        printHex((uint64)curr);
        printString(" size=");
        printHex(curr->size);
        printString("]\n");
        curr = curr->next;
        if (++i > 10) break;
    }
    printString("---\n");
}