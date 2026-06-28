#include "../h/syscall_c.hpp"
#include "../h/utils.hpp"

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
        "mv a2, %2\n"
        "mv a1, %1\n"
        "li a0, 0x11\n"
        "ecall\n"
        "mv %0, a0"
        : "=r"(reg)
        : "r"(start_routine), "r"(arg)
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
int sem_open(sem_t* handle, unsigned init) {
    uint64 reg;
    asm volatile(
        "mv a1, %1\n"
        "li a0, 0x21\n"
        "ecall\n"
        "mv %0, a0"
        : "=r"(reg)
        : "r"(init)
        : "a0", "a1", "memory"
    );
    *handle = (sem_t)reg;
    return (*handle != nullptr ? 0 : -1);
}

int sem_close(sem_t handle) {
    uint64 reg;
    asm volatile(
        "mv a1, %1\n"
        "li a0, 0x22\n"
        "ecall\n"
        "mv %0, a0"
        : "=r"(reg)
        : "r"(handle)
    );

    return (int)reg;
}

int sem_wait(sem_t id) {
    uint64 reg;
    asm volatile(
        "mv a1, %1\n"
        "li a0, 0x23\n"
        "ecall\n"
        "mv %0, a0"
        : "=r"(reg)
        : "r"(id)
    );

    return (int)reg;
}
int sem_signal(sem_t id) {
    uint64 reg;
    asm volatile(
        "mv a1, %1\n"
        "li a0, 0x24\n"
        "ecall\n"
        "mv %0, a0"
        : "=r"(reg)
        : "r"(id)
    );

    return (int)reg;
}

int sem_wait_n(sem_t id, unsigned n) {
    uint64 reg;
    asm volatile (
        "mv a2, %2\n"
        "mv a1, %1\n"
        "li a0, 0x25\n"
        "ecall\n"
        "mv %0, a0"
        : "=r"(reg)
        : "r"(id), "r"(n)
    );

    return (int)reg;
}
int sem_signal_n(sem_t id, unsigned n) {
    uint64 reg;
    asm volatile (
        "mv a2, %2\n"
        "mv a1, %1\n"
        "li a0, 0x26\n"
        "ecall\n"
        "mv %0, a0"
        : "=r"(reg)
        : "r"(id), "r"(n)
    );
    return (int)reg;
}

int time_sleep(time_t time) {
    uint64 reg;
    asm volatile (
        "mv a1, %1\n"
        "li a0, 0x31\n"
        "ecall\n"
        "mv %0, a0"
        : "=r"(reg)
        : "r"((uint64)time)
    );
    return (int)reg;
}

char getc() {
    uint64 reg;
    asm volatile (
        "li a0, 0x41\n"
        "ecall\n"
        "mv %0, a0"
        : "=r"(reg)
    );
    char c = (char) reg;
    if(c == '\r') c = '\n';
    return c;
}

void putc(char c) {
    uint64 reg;
    asm volatile (
        "mv a1, %1\n"
        "li a0, 0x42\n"
        "ecall\n"
        "mv %0, a0"
        : "=r"(reg)
        : "r"((uint64)c)
    );
}