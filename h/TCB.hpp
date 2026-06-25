#ifndef UGOS_TCB_H
#define UGOS_TCB_H

#include "../lib/hw.h"
#include "MemoryAllocator.hpp"

class TCB {
    using Body = void(*)(void*);
    
public:

    static TCB* running;
    
    static TCB* create(Body body, void* arg);
    static int exit();
    static void dispatch();

    inline bool isFinished() const { return finished;}
    void* kernelStack;
    uint64 sp;
    
private:
    //TCB(Body body, void* arg);

    static void start();

    void* userStack;
    
    Body body;
    void* arg;

    bool finished;
    TCB* next;

    friend class Scheduler;
};

#endif // UGOS_TCB_H