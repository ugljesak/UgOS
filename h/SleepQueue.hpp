#ifndef UGOS_SLEEP_QUEUE_HPP
#define UGOS_SLEEP_QUEUE_HPP

#include "../lib/hw.h"
#include "TCB.hpp"


class Scheduler;

struct SleepNode {
    SleepNode(TCB* thread, uint64 dist)
        : thread(thread), dist(dist), next(nullptr) {}
    TCB* thread;
    uint64 dist;
    SleepNode* next;
};

class SleepQueue {
public:
    SleepQueue() : head(nullptr) {}    

    void insert(TCB* t, uint64 time);
    void tick();
    
private:
    SleepNode* head;

    friend class Scheduler;
};

#endif // UGOS_SLEEP_QUEUE_HPP