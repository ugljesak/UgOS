#include "../h/_Sem.hpp"
#include "../h/syscall_c.hpp"
#include "../h/Scheduler.hpp"

int _Sem::wait() {
    if(closed) return -1;
    if(--val < 0) {
        TCB::running->block();
        blocked.push(TCB::running);
        TCB::dispatch();
        if(closed) return -2;
    }
    return 0;
}

int _Sem::waitN(unsigned n) {
    if(closed) return -1;
    val -= (int)n;
    if(val < 0) {
        blocked.push(TCB::running);
        TCB::running->block();
        TCB::dispatch();
        if(closed) return -2;
    }
    return 0;
}

int _Sem::signal() {
    if(++val == 0) {
        TCB* tcb = blocked.pop();
        if(tcb) {
            tcb->unblock();
            Scheduler::put(tcb);
        }
    }
    return 0;
}

int _Sem::signalN(unsigned n) {
    val += (int)n;
    while(val == 0) {
        TCB* tcb = blocked.pop();
        if(tcb == nullptr) break;
        tcb->unblock();
        Scheduler::put(tcb);
    }
    return 0;
}

int _Sem::close() {
    closed = true;
    while(true) {
        TCB* tcb = blocked.pop();
        if(!tcb) break;
        tcb->unblock();
        Scheduler::put(tcb);
    }
    TCB::dispatch();
    return 0;
}