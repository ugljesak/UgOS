#ifndef UGOS_SCHEDULER_H
#define UGOS_SCHEDULER_H

#include "TCB.hpp"

class Scheduler {

public:
    static void put(TCB* thread);
    static TCB* get();
    
    static void printQueue();
private:
    static TCB *head, *tail;    
};

#endif // UGOS_SCHEDULER_H