#include "../h/syscall_c.hpp"


void* mem_alloc(size_t size) {
    // ceil(size/MEM_BLOCK_SIZE)
    size_t numBlocks = (size + MEM_BLOCK_SIZE -1) / MEM_BLOCK_SIZE;
    uint64 reg;
    asm volatile (
        "mv a1, %1\n"
        "li a0, 0x01\n"
        "ecall\n"
        "mv %0, a0"
        : "=r"(reg)
        : "r"(numBlocks)
    );

    return (void*)reg;

    // return (void*)ecall(0x01, (uint64)numBlocks);
}

int mem_free(void* ptr) {
    
    uint64 reg;
    asm volatile (
        "mv a1, %1\n"
        "li a0, 0x02\n"
        "ecall\n"
        "mv %0, a0"
        : "=r"(reg)
        : "r"(ptr)
    );
    
    return reg;
    // return (int)ecall(0x02, (uint64)ptr);
}

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg) {
    uint64 reg;
    asm volatile (
        "mv a3, %3\n"
        "mv a2, %2\n"
        "mv a1, %1\n"
        "li a0, 0x11\n"
        "ecall\n"
        "mv %0, a0"
        : "=r"(reg)
        : "r"(handle), "r"(start_routine), "r"(arg)
    );

    *handle = (TCB*)reg;
    return (*handle != nullptr ? 0 : -1);
}

int thread_exit() {
    uint64 reg;
    asm volatile (
        "li a0, 0x12\n"
        "ecall\n"
        "mv %0, a0\n"
        : "=r"(reg)
    );

    return (int)reg;
}

void thread_dispatch() {
    asm volatile (
        "li a0, 0x13\n"
        "ecall"
    );
}

