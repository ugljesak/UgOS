#include "../h/syscall_c.hpp"

void* operator new(size_t size) {
    return mem_alloc(size);
}

void operator delete(void* ptr) {
    mem_free(ptr);
}

void* operator new[](size_t size) {
    return mem_alloc(size);
}

void operator delete[](void* ptr) {
    mem_free(ptr);
}

// C++14+ zahteva i ovu verziju
void operator delete(void* ptr, size_t) {
    mem_free(ptr);
}

void operator delete[](void* ptr, size_t) {
    mem_free(ptr);
}