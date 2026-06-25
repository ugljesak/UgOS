#include "../h/Scheduler.hpp"
#include "../h/utils.hpp"

TCB* Scheduler::head = nullptr;
TCB* Scheduler::tail = nullptr;

void Scheduler::put(TCB* thread) {
    thread->next = nullptr;
    if(tail == nullptr) {
        head = thread;
        tail = thread;
        head->next = tail;
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
        printValue("sp", t->sp);                                         
        printValue("fin", t->finished); 
        printLine("|");
        printLine("v");
        t = t->next;
        ++i;
    }
    while(t != nullptr);
    printValue("total", i);
}