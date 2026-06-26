#include "../h/TCB.hpp"
#include "../h/Scheduler.hpp"
#include "../h/utils.hpp"


void yield(TCB* oldThread, TCB* newThread);
void userThreadStart(void*);

TCB* TCB::running = nullptr;
TCB* TCB::toFree = nullptr;

void TCB::start() {

    void* userSP = (char*)running->userStack + 64 * DEFAULT_STACK_SIZE - 16;
    
    asm volatile("csrw sscratch, %0" :: "r"(running->sp));
    asm volatile(
        "mv sp, %0\n"
        "mv a0, %1\n"
        "jalr %2\n"
        : : "r"(userSP), "r"(running->arg), "r"(running->body)
    );
    exit();
}

TCB* TCB::create(Body body, void* arg) {
    
    TCB* tcb = (TCB*)MemoryAllocator::mem_alloc(sizeof(TCB));
    
    tcb->kernelStack = MemoryAllocator::mem_alloc(64 * DEFAULT_STACK_SIZE);
    tcb->userStack = MemoryAllocator::mem_alloc(64 * DEFAULT_STACK_SIZE);
    
    tcb->body = body;
    tcb->arg = arg;

    tcb->finished = false;
    tcb->next = nullptr;

    tcb->sp  = (uint64)((char*)tcb->kernelStack + 64 * DEFAULT_STACK_SIZE - 112);

    // uint64* frame = (uint64*)tcb->sp;                                        
    // frame[0] = (uint64)&TCB::start;  // ra                                   
    // for (int i = 1; i < 13; i++) frame[i] = 0;  // s0-s11
              

    asm volatile(
        "sd %0, 0(%1)\n"
        "sd x0, 8(%1)\n"
        "sd x0, 16(%1)\n"
        "sd x0, 24(%1)\n"
        "sd x0, 32(%1)\n"
        "sd x0, 40(%1)\n"
        "sd x0, 48(%1)\n"
        "sd x0, 56(%1)\n"
        "sd x0, 64(%1)\n"
        "sd x0, 72(%1)\n"
        "sd x0, 80(%1)\n"
        "sd x0, 88(%1)\n"
        "sd x0, 96(%1)\n"
        : : "r"((uint64)&start), "r"(tcb->sp)
    );

    Scheduler::put(tcb);

    return tcb;
}

TCB* TCB::createUserThread(Body body, void* arg) {
    TCB* tcb = (TCB*)MemoryAllocator::mem_alloc(sizeof(TCB));
    
    tcb->kernelStack = MemoryAllocator::mem_alloc(64 * DEFAULT_STACK_SIZE);
    tcb->userStack = MemoryAllocator::mem_alloc(64 * DEFAULT_STACK_SIZE);
    
    tcb->body = body;
    tcb->arg = arg;

    tcb->finished = false;
    tcb->next = nullptr;

    uint64 ksp = (uint64)((char*)tcb->kernelStack + 64 * DEFAULT_STACK_SIZE);
    ksp &= ~0xFUL;

    uint64 usp = (uint64)((char*)tcb->userStack + 64 * DEFAULT_STACK_SIZE);
    usp &= ~0xFUL;

    ksp -= 13;
    uint64* frame = (uint64*)ksp;
    frame[0] = (uint64)userThreadStart; //ra
    frame[1] = (uint64)body; //s0
    frame[2] = usp; //s1
    frame[3] = (uint64)arg; //s2
    frame[4] = ksp + 13; //s3
    for(int i = 5; i < 13; ++i) {
        frame[i] = 0;
    }

    tcb->sp =ksp;
    Scheduler::put(tcb);
    return tcb;
}

void TCB::lazyFree() {
    if(toFree) {
        MemoryAllocator::mem_free(toFree->userStack);
        MemoryAllocator::mem_free(toFree->kernelStack);
        MemoryAllocator::mem_free(toFree);
        toFree = nullptr;
    }
}

int TCB::exit() {
    running->finished = true;

    lazyFree();
    toFree = running;

    TCB* oldT = running;
    TCB* newT = Scheduler::get();
    running = newT;

    yield(oldT, newT);
    return 0;
}

void TCB::dispatch() {
    lazyFree();

    TCB* oldT = TCB::running;
    Scheduler::put(oldT);

    TCB* newT = Scheduler::get();
    TCB::running = newT;
    //newT->debugPrint();
    yield(oldT, newT);
}

extern void hook();

void TCB::init() {
    TCB* mainT = (TCB*)MemoryAllocator::mem_alloc(sizeof(TCB));
    
    mainT->kernelStack = MemoryAllocator::mem_alloc(64 * DEFAULT_STACK_SIZE);
    mainT->userStack = nullptr;
    // mainT->userStack = MemoryAllocator::mem_alloc(64 * DEFAULT_STACK_SIZE);
    
    mainT->sp = (uint64)((char*)mainT->kernelStack + 64 * DEFAULT_STACK_SIZE - 112);
    // uint64 userSP = (uint64)((char*)mainT->userStack + 64 * DEFAULT_STACK_SIZE - 112);
    mainT->body = nullptr;
    mainT->arg = nullptr;
    mainT->next = nullptr;
    mainT->finished = false;


    // asm volatile("mv sp, %0" : : "r"(userSP));
    asm volatile("csrw sscratch, %0" : : "r"(mainT->sp));
    // #include <cstddef>
    // static_assert(offsetof(TCB, sp) == 0, "sp offset wrong");
    
    TCB::running = mainT;
}

void TCB::debugPrint() {
    printString("=== TCB DEBUG ===\n");
    printString("TCB addr:      "); printHex((uint64)this);        printString("\n");
    printString("&sp:           "); printHex((uint64)&sp);         printString("\n");
    printString("sp val:        "); printHex(sp);                  printString("\n");
    printString("&kernelStack:  "); printHex((uint64)&kernelStack);printString("\n");
    printString("kernelStack:   "); printHex((uint64)kernelStack); printString("\n");
    printString("&userStack:    "); printHex((uint64)&userStack);  printString("\n");
    printString("userStack:     "); printHex((uint64)userStack);   printString("\n");
    printString("&body:         "); printHex((uint64)&body);       printString("\n");
    printString("body:          "); printHex((uint64)body);        printString("\n");
    printString("&finished:     "); printHex((uint64)&finished);   printString("\n");
    printString("finished:      "); printHex((uint64)finished);    printString("\n");
    printString("&next:         "); printHex((uint64)&next);       printString("\n");
    printString("next:          "); printHex((uint64)next);        printString("\n");
    printString("offsetof sp:   "); printHex((uint64)&sp   - (uint64)this); printString("\n");
    printString("offsetof kStk: "); printHex((uint64)&kernelStack - (uint64)this); printString("\n");
    printString("offsetof uStk: "); printHex((uint64)&userStack   - (uint64)this); printString("\n");
    printString("sp - kStk base:"); printHex(sp - (uint64)kernelStack); printString("\n");
    printString("=================\n");
}