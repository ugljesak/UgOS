#include "../h/syscall_c.hpp"

inline uint64 ecall(uint64 code, uint64 a1 = 0UL, uint64 a2 = 0UL, uint64 a3 = 0UL, uint64 a4 = 0UL) {
    
    uint64 res;
    asm volatile (
        "ecall\n"
        "mv %0, a0"
        : "=r" (res)
    );

    return res;
}

void* mem_alloc(size_t size) {
    // ceil(size/MEM_BLOCK_SIZE)
    size_t numBlocks = (size + MEM_BLOCK_SIZE -1) / MEM_BLOCK_SIZE;
    uint64 reg;
    asm volatile (
        "mv a1, %1\n"
        "li a0, 0x01\n"
        "ecall\n"
        "mv %0, a0"
        : "=r" (reg)
        : "r" (numBlocks)
    );

    return (void*)reg;
}

int mem_free(void* ptr) {
    
    uint64 reg;
    asm volatile (
        "mv a1, %1\n"
        "li a0, 0x02\n"
        "ecall\n"
        "mv %0, a0"
        : "=r" (reg)
        : "r" (ptr)
    );

    return reg;
    //return (int)ecall(0x02, (uint64)ptr);
}
