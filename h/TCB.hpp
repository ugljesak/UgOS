#ifndef UGOS_TCB_H
#define UGOS_TCB_H

#include "../lib/hw.h"
#include "MemoryAllocator.hpp"


struct Context {
    uint64 ra;
    uint64 sp;
    //uint64 s[12];
};

class TCB {
    using Body = void(*)(void*);
    
public:
    static TCB* running;
    static int counter;
    
    static TCB* create(Body body, void* arg);
    static TCB* createMain(Body body, void* arg);
    //static TCB* createUserThread(Body body, void* arg);
    static void dispatch();
    
    inline Context* getContext() {return &context;}

    inline bool isFinished() const {return finished;}
    
    inline uint64 getTimeSlice() const { return timeSlice;}
    inline static uint64 getTimeCounter() { return timeCounter;}
    inline static void incTimeCounter() {timeCounter++;}
    inline static void resetTimeCounter() {timeCounter = 0;}

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

    TCB(Body body, void* arg, uint64 timeSlice);
private:

    static void lazyFree();
    static TCB* toFree;
    static uint64 timeCounter;
    
    uint64* userStack;
    Context context;

    int id;
    uint64 timeSlice;

    Body body;
    void* arg;
    
    TCB* next = nullptr;
    bool finished = false;
    
    

    static void threadWrapper();
    static void yield(::Context* oldContext, ::Context* newContext);

    friend class Scheduler;
};

#endif // UGOS_TCB_H