#include "../h/Scheduler.hpp"
#include "../h/utils.hpp"
#include "../h/syscall_c.hpp"

TCB* Scheduler::head = nullptr;
TCB* Scheduler::tail = nullptr;
SleepQueue Scheduler::sleepQueue = SleepQueue();

void Scheduler::put(TCB* thread) {
    thread->next = nullptr;
    if(tail == nullptr) {
        head = thread;
        tail = thread;
        //head->next = tail;
        return;    
    }

    tail->next = thread;
    tail = thread;
}


TCB* Scheduler::get() {
    if(head == nullptr) {
        return nullptr;
    }

    TCB* t = head;
    head = head->next;
    if(head == nullptr){
        tail = nullptr;
    }
    
    return t;
}

void Scheduler::sleepTick() {
    if(sleepQueue.head != nullptr) {
        //printValue("tick dist", sleepQueue.head->dist);
        sleepQueue.tick();
    }
}

int Scheduler::sleep(TCB* thread, uint64 ticks) {
    if(ticks <= 0) return -1;
    thread->blocked = true;
    sleepQueue.insert(thread, ticks);
    TCB::dispatch();
    return 0;
}

void Scheduler::printQueue() {
    TCB* t = head;
    int i = 0;
    do {
        printValue("TCB_id", (uint64)t->id);
        printValue("body", (uint64)t->body);
        printValue("user stack", (uint64)t->userStack);
        // printValue("kernel stack", (uint64)t->kernelStack);
        printValue("ra", (uint64)t->context.ra);
        printValue("sp", t->context.sp);
        printValue("fin", t->finished);
        printLine("|");
        printLine("v");
        t = t->next;
        ++i;
    }
    while(t != nullptr);
    printValue("total", i);
}