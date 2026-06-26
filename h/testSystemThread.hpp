#ifndef UGOS_TESTSYSTEMTHREAD_HPP
#define UGOS_TESTSYSTEMTHREAD_HPP

#include "syscall_c.hpp"
#include "TCB.hpp"
#include "printing.hpp"
#include "Scheduler.hpp"

static volatile bool finishedA = false;
static volatile bool finishedB = false;
static volatile bool finishedC = false;
static volatile bool finishedD = false;

static uint64 fibonacci(uint64 n) {
    if (n == 0 || n == 1) { return n; }
    if (n % 10 == 0) { thread_dispatch(); }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

static void workerBodyA(void* arg) {
    printLine("Usao u telo A");
    for (uint64 i = 0; i < 10; i++) {
        printString("A: i="); printInt(i); printString("\n");
        for (uint64 j = 0; j < 10000; j++) {
            for (uint64 k = 0; k < 30000; k++) { /* busy wait */ }
            thread_dispatch();
        }
    }
    printString("A finished!\n");
    finishedA = true;
}

static void workerBodyB(void* arg) {
    printLine("Usao u telo B");
    for (uint64 i = 0; i < 16; i++) {
        printString("B: i="); printInt(i); printString("\n");
        for (uint64 j = 0; j < 10000; j++) {
            for (uint64 k = 0; k < 30000; k++) { /* busy wait */ }
            thread_dispatch();
        }
        if (i == 10) {
            asm volatile("csrr t6, sepc");
        }
    }
    printString("B finished!\n");
    finishedB = true;
    thread_dispatch();
}

static void workerBodyC(void* arg) {
    uint8 i = 0;
    for (; i < 3; i++) {
        printString("C: i="); printInt(i); printString("\n");
    }

    printString("C: dispatch\n");
    __asm__ ("li t1, 7");
    thread_dispatch();

    uint64 t1 = 0;
    __asm__ ("mv %[t1], t1" : [t1] "=r"(t1));

    printString("C: t1="); printInt(t1); printString("\n");

    uint64 result = fibonacci(12);
    printString("C: fibonaci="); printInt(result); printString("\n");

    for (; i < 6; i++) {
        printString("C: i="); printInt(i); printString("\n");
    }

    printString("C finished!\n");
    finishedC = true;
    thread_dispatch();
}

static void workerBodyD(void* arg) {
    uint8 i = 10;
    for (; i < 13; i++) {
        printString("D: i="); printInt(i); printString("\n");
    }

    printString("D: dispatch\n");
    __asm__ ("li t1, 5");
    thread_dispatch();

    uint64 result = fibonacci(16);
    printString("D: fibonaci="); printInt(result); printString("\n");

    for (; i < 16; i++) {
        printString("D: i="); printInt(i); printString("\n");
    }

    printString("D finished!\n");
    finishedD = true;
    thread_dispatch();
}


void System_Mode_test() {
    thread_t threads[4];

    thread_create(&threads[0], workerBodyA, nullptr);
    if(threads[0] != nullptr) printLine("ThreadA created");

    thread_create(&threads[1], workerBodyB, nullptr);
    if(threads[1] != nullptr) printLine("ThreadB created");

    thread_create(&threads[2], workerBodyC, nullptr);
    if(threads[2] != nullptr) printString("ThreadC created\n");

    thread_create(&threads[3], workerBodyD, nullptr);
    if(threads[3] != nullptr) printString("ThreadD created\n");
    
    printLine("-----------------------");
    Scheduler::printQueue();
    printLine("-----------------------");

    while (!(finishedA && finishedB && finishedC && finishedD)) {
        // printReg("sp", 0);
        // printReg("sscratch", 1);

        thread_dispatch();
    }

}


#endif // UGOS_TESTSYSTEMTHREAD_HPP