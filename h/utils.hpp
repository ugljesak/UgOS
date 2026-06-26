#ifndef UGOS_UTILS_HPP
#define UGOS_UTILS_HPP

#include "../lib/console.h"
#include "../lib/hw.h"

#define printReg(name, priv) \
    do { \
        uint64 _reg; \
        if (priv) \
            asm volatile("csrr %0, " name : "=r"(_reg)); \
        else \
            asm volatile("mv %0, " name : "=r"(_reg)); \
        printValue(name, _reg); \
    } while(0)

inline void printString(const char* str) {
    while(*str != '\0') {
        __putc(*(str++));
    }
}

inline void printLine(const char* str) {
    printString(str);
    __putc('\n');
}

inline void printDec(uint64 val) {
    char s[64];
    int i=63;
    while(val) {
        s[i--] = val%10+'0';
        val/=10;
    }
    printLine(s+i);
}

inline void printHex(uint64 val) {
    const char* hex = "0123456789abcdef";
    printString("0x");
    for (int i=60; i>=0; i-=4) {
        __putc(hex[(val >> i) & 0xF]);
    }
}

inline void printValue(const char* name, uint64 val) {
    printString(name);
    printString(" = ");
    printHex(val);
    __putc('\n');
}

#endif // UGOS_UTILS_HPP