#ifndef UGOS__SEM_H
#define UGOS__SEM_H

#include "../lib/hw.h"
#include "List.hpp"
#include "../h/TCB.hpp"
#include "../h/Scheduler.hpp"

class _Sem {
public:
    _Sem(unsigned n = 1)
        : val(n), blocked(List<TCB*>()), closed(false) {}
    
    int wait();
    int waitN(unsigned n);
    
    int signal();
    int signalN(unsigned n);

    int signalFromISR() {
    if(++val == 0) {
        TCB* tcb = blocked.pop();
        if(tcb) {
            tcb->unblock();
            Scheduler::put(tcb);
        }
    }
    return 0;
}

private:
    int close();

    int val;
    List<TCB*> blocked;
    bool closed;

    friend class Handler;
};

#endif // UGOS__SEM_H