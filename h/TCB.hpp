#ifndef UGOS_TCB_H
#define UGOS_TCB_H

#include "../lib/hw.h"
#include "MemoryAllocator.hpp"

class TCB {
    using Body = void(*)(void*);
    
public:
    static TCB* running;
    static TCB* toFree;

    static TCB* create(Body body, void* arg);
    static int exit();
    static void dispatch();

    inline bool isFinished() const { return finished;}
    
    static void init();
    void debugPrint();

private:
    //TCB(Body body, void* arg);
    static void start();
    static void lazyFree();

    void* kernelStack = nullptr; //0 48080 [480c0 - 880bf]
    void* userStack = nullptr; //8 48090 [880d0 - c80cf]
    Body body = nullptr; //16 48078
    void* arg = nullptr; //24 48088
    
    TCB* next = nullptr; //32 48098
    uint64 sp = 0; //40 480a0 [88050]    

    bool finished = false; //48 480a8

    friend class Scheduler;
};

#endif // UGOS_TCB_H