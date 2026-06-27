#include "../h/TCB.hpp"
#include "../h/Scheduler.hpp"
#include "../h/utils.hpp"
#include "../h/Controller.hpp"


void userThreadStart();

TCB* TCB::running = nullptr;
TCB* TCB::toFree = nullptr;
uint64 TCB::timeCounter = 0;
int TCB::counter = 0;

void TCB::threadWrapper() {
    Controller::sReturn();
    printValue("id", running->id);
    if (running->body != nullptr) {
        running->body(running->arg);
    }
    running->finished = true; 
    dispatch();
}

TCB::TCB(Body body, void* arg, uint64 timeSlice) : 
    userStack(body ? (uint64*)MemoryAllocator::mem_alloc(DEFAULT_STACK_SIZE) : nullptr),
    id(counter++),
    timeSlice(timeSlice),
    body(body),
    arg(arg),
    next(nullptr),
    finished(false)
{
    
}

TCB* TCB::create(Body body, void* arg) {
    TCB* tcb = new TCB(body, arg, DEFAULT_TIME_SLICE);
    tcb->context.ra = (uint64)&threadWrapper;
    tcb->context.sp = (uint64)((char*)tcb->userStack + DEFAULT_STACK_SIZE);
    tcb->context.sp &= ~0xF;
    for(int i = 0; i < 12; ++i) {
        tcb->context.s[i] = 0;
    }
    Scheduler::put(tcb);
    return tcb;    
}
TCB* TCB::createMain(Body body, void* arg) {
    TCB* tcb = new TCB(body, arg, DEFAULT_TIME_SLICE );
    //asm volatile("mv %0, ra" : "=r"(tcb->context.ra));
    //asm volatile("mv %0, sp" : "=r"(tcb->context.sp));
    return tcb;
}


void TCB::lazyFree() {
    if(toFree) {
        if(toFree->userStack) MemoryAllocator::mem_free(toFree->userStack);
        // if(toFree->kernelStack) MemoryAllocator::mem_free(toFree->kernelStack);
        MemoryAllocator::mem_free(toFree);
        toFree = nullptr;
    }
}


void TCB::dispatch() {
    lazyFree();

    TCB* oldT = TCB::running;
    if(!oldT->isFinished()) {
        Scheduler::put(oldT);
    }
    TCB* newT = Scheduler::get();
    
    //printValue("oldT.id", oldT->id);
    //printValue("newT.id", newT->id);
    if (oldT != newT) {
        // if(running->id == 0) {
        //     Controller::mask_set_sstatus(Controller::SSTATUS_SPP);
        // }
        // else {
        //     Controller::mask_clear_sstatus(Controller::SSTATUS_SPP);
        // }
        TCB::running = newT;
        yield(&oldT->context, &newT->context);
    }
}

void TCB::init() {
    
    // mainT->context.sp = (uint64)((char*)mainT->kernelStack + DEFAULT_STACK_SIZE - 112);
    
    //Controller::mask_set_sstatus(Controller::::SSTATUS_SIE);
}

void TCB::debugPrint() {
    printString("=== TCB DEBUG ===\n");
    printString("TCB addr:      "); printHex((uint64)this);        printString("\n");
    printString("&sp:           "); printHex((uint64)&context.sp); printString("\n");
    printString("sp val:        "); printHex(context.sp);          printString("\n");
    // printString("&kernelStack:  "); printHex((uint64)&kernelStack);printString("\n");
    // printString("kernelStack:   "); printHex((uint64)kernelStack); printString("\n");
    printString("&userStack:    "); printHex((uint64)&userStack);  printString("\n");
    printString("userStack:     "); printHex((uint64)userStack);   printString("\n");
    printString("&body:         "); printHex((uint64)&body);       printString("\n");
    printString("body:          "); printHex((uint64)body);        printString("\n");
    printString("&finished:     "); printHex((uint64)&finished);   printString("\n");
    printString("finished:      "); printHex((uint64)finished);    printString("\n");
    printString("&next:         "); printHex((uint64)&next);       printString("\n");
    printString("next:          "); printHex((uint64)next);        printString("\n");
    printString("offsetof sp:   "); printHex((uint64)&context.sp  - (uint64)this); printString("\n");
    // printString("offsetof kStk: "); printHex((uint64)&kernelStack - (uint64)this); printString("\n");
    printString("offsetof uStk: "); printHex((uint64)&userStack   - (uint64)this); printString("\n");
    // printString("sp - kStk base:"); printHex(context.sp - (uint64)kernelStack); printString("\n");
    printString("=================\n");
}