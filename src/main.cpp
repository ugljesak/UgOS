#include "../lib/hw.h"
#include "../h/TCB.hpp"
#include "../h/Scheduler.hpp"
#include "../h/utils.hpp"
#include "../h/syscall_c.hpp"
#include "../h/Controller.hpp"

void trap_entry(); 
extern void userMain(void*);

int main() {
    MemoryAllocator::init();
    Controller::write_stvec((uint64)&trap_entry);
    //asm volatile("csrw stvec, %0" : : "r"((uint64)&trap_entry));
    //TCB::init();
    
    //TCB* mainT = TCB::createMain(nullptr, nullptr);
    TCB* mainT = new TCB(nullptr, nullptr);
    TCB::running = mainT;
    
    TCB* user = TCB::create(userMain, nullptr);
    //Scheduler::printQueue();
    while(!user->isFinished()) {
        
        thread_dispatch();
    }

    Controller::exit();
    return 0;
}