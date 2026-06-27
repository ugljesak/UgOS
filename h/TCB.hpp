#ifndef UGOS_TCB_H
#define UGOS_TCB_H

#include "../lib/hw.h"
#include "MemoryAllocator.hpp"


struct Context {
    uint64 ra;
    uint64 sp;
    uint64 s[12];
};

class TCB {
    using Body = void(*)(void*);
    
public:
    static TCB* running;
    static TCB* toFree;
    static uint64 timeSlice;
    static uint64 timeCounter;
    static int counter;
    
    static TCB* create(Body body, void* arg);
    static TCB* createMain(Body body, void* arg);
    //static TCB* createUserThread(Body body, void* arg);
    static int exit();
    static void dispatch();
    
    inline Context* getContext() {return &context;}

    inline bool isFinished() const { return finished;}
    
    static void init();
    void debugPrint();

    void* operator new(size_t size) {
        return MemoryAllocator::mem_alloc(size);
    }
    void* operator new[](size_t size) {
        return MemoryAllocator::mem_alloc(size);
    }
    
    void operator delete(void *ptr) {
        MemoryAllocator::mem_free(ptr);
    }
    void operator delete[](void *ptr) {
        MemoryAllocator::mem_free(ptr);
    }

    ~TCB() { 
        delete[] userStack;
    }

    TCB(Body body, void* arg);
private:

    static void lazyFree();

    uint64* userStack = nullptr; //8 48090 [880d0 - c80cf]
    Context context;

    Body body = nullptr; //16 48078
    void* arg = nullptr; //24 48088

    //uint64* kernelStack = nullptr; //0 48080 [480c0 - 880bf]
    
    TCB* next = nullptr; //32 48098
    bool finished = false; //48 480a8
    int id;

    static void threadWrapper();
    //static void userThreadWrapper();
    static void yield(::Context* oldContext, ::Context* newContext);

    friend class Scheduler;
};

#endif // UGOS_TCB_H