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
    
    TCB* mainT = new TCB(nullptr, nullptr, DEFAULT_TIME_SLICE);
    TCB::running = mainT;
    TCB* user = TCB::create(userMain, nullptr);
    
    Controller::mask_set_sstatus(Controller::SSTATUS_SIE);
    
    while(!(user)->isFinished()) {
        
        thread_dispatch();
    }

    Controller::exit();
    return 0;
}