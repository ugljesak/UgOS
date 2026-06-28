#include "../lib/hw.h"
#include "../h/TCB.hpp"
#include "../h/Scheduler.hpp"
#include "../h/utils.hpp"
#include "../h/syscall_c.hpp"
#include "../h/Controller.hpp"
#include "../h/OutputWorker.hpp"
#include "../h/InputWorker.hpp"


extern void trap_entry(); 
extern void userMain(void*);

int main() {
    MemoryAllocator::init();
    Controller::write_stvec((uint64)&trap_entry);
    uint64 stvec_val;
    asm volatile("csrr %0, stvec" : "=r"(stvec_val));
    
    TCB* mainT = new TCB(nullptr, nullptr);
    TCB::running = mainT;
    
    OutputWorker::init();
    InputWorker::init();

    Controller::mask_set_sstatus(Controller::SSTATUS_SIE);
    TCB* user = TCB::create(userMain, nullptr);
    
    
    while(!OutputWorker::outputBuffer->isEmpty()) {
        thread_dispatch();
    }
    while(!(user)->isFinished()) {
        
        thread_dispatch();
    }

    Controller::exit();
    return 0;
}