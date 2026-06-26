#include "../h/Scheduler.hpp"
#include "../h/utils.hpp"

TCB* Scheduler::head = nullptr;
TCB* Scheduler::tail = nullptr;

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


void Scheduler::printQueue() {
    TCB* t = head;
    int i = 0;
    do {
        printValue("TCB", (uint64)t);
        printValue("body", (uint64)t->body);
        printValue("user stack", (uint64)t->userStack);
        printValue("kernel stack", (uint64)t->kernelStack);
        printValue("sp", t->sp);
        printString("*t->sp = "); printHex((uint64)(*((uint64*)(t->sp)))); printString("\n");
        printValue("fin", t->finished);
        printLine("|");
        printLine("v");
        t = t->next;
        ++i;
    }
    while(t != nullptr);
    printValue("total", i);
}