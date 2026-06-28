#ifndef UGOS_SCHEDULER_H
#define UGOS_SCHEDULER_H

#include "TCB.hpp"
#include "SleepQueue.hpp"

class Scheduler {

public:
    static void put(TCB* thread);
    static TCB* get();
    
    static void printQueue();

    static void sleepTick();
    static int sleep(TCB* thread, uint64 ticks);

private:
    static TCB *head, *tail;
    static SleepQueue sleepQueue;
};

#endif // UGOS_SCHEDULER_H