#include "../h/TCB.hpp"
#include "../h/Scheduler.hpp"


void yield(TCB* oldThread, TCB* newThread);

TCB* TCB::running = nullptr;

void TCB::start() {
    uint64 userSP = (uint64)((char*)running->userStack + 64 * DEFAULT_STACK_SIZE - 104);
    asm volatile("csrw sscratch, %0" : : "r"(userSP));

    TCB::running->body(TCB::running->arg);
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

    tcb->sp  = (uint64)((char*)tcb->kernelStack + 64 * DEFAULT_STACK_SIZE - 104);
    
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

int TCB::exit() {
    TCB::running->finished = true;
    MemoryAllocator::mem_free(TCB::running->userStack);
    
    TCB* oldT = TCB::running;
    TCB* newT = Scheduler::get();
    TCB::running = newT;

    yield(oldT, newT);

    return 0;
}

void TCB::dispatch() {
    
    TCB* oldT = TCB::running;
    Scheduler::put(oldT);

    TCB* newT = Scheduler::get();
    TCB::running = newT;

    yield(oldT, newT);
}