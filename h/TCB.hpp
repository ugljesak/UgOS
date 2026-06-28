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
    static int counter;
    
    static TCB* create(Body body, void* arg);
    static TCB* createDaemon(Body body, void* arg);
    //static TCB* createUserThread(Body body, void* arg);
    
    //inline Context* getContext() {return &context;}

    inline bool isFinished() const {return finished;}
    inline void setFinished() {finished = true;}

    inline bool isBlocked() const {return blocked;}
    inline void block() {blocked = true;}
    inline void unblock() {blocked = false;}

    inline bool isDaemon() const {return daemon;}

    inline uint64 getTimeSlice() const { return timeSlice;}
    
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

    TCB(Body body, void* arg, uint64 timeSlice = DEFAULT_TIME_SLICE);
private:
    static void dispatch();

    static void lazyFree();
    static TCB* toFree;
    static uint64 timeCounter;
    
    uint64* userStack;
    Context context;

    int id;
    uint64 timeSlice;

    Body body;
    void* arg;
    
    TCB* next;
    bool finished;
    
    bool blocked;
    bool semClosed;

    bool daemon;

    static void threadWrapper();
    static void yield(::Context* oldContext, ::Context* newContext);

    friend class Scheduler;
    friend class Handler;
    friend class _Sem;
    friend class ConsoleBuffer;
};

#endif // UGOS_TCB_H