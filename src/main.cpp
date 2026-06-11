//
// Created by ugoboss on 5/15/26.
//
#include "../lib/hw.h"
#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"

int main() {
    char ch = __getc();
    __putc(ch);
    __putc('\n');
    void* test = nullptr;
    MemoryAllocator::mem_free(test);

    return 0;
}