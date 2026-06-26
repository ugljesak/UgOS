#include "../h/MemoryAllocator.hpp"
#include "../h/TCB.hpp"
#include "../h/utils.hpp"


struct Frame {
    uint64 ra;
    uint64 sp;
    uint64 gp;
    uint64 tp;
    uint64 t[7];
    uint64 s[12];
    uint64 a[8];
    uint64 sepc;
    uint64 sstatus;
};

uint64 handle_syscall(uint64 a0, uint64 a1, uint64 a2, uint64 a3);

uint64 handle_trap(uint64 cause, uint64 a0, uint64 a1, uint64 a2, uint64 a3, uint64 sp) { 
    //printValue("TRAP cause", cause);
    if(cause & (1UL << 63)) {
        //TCB::dispatch();
        return 0UL;
    }
    else{
        // Unutrasnji prekid
        switch(cause) {
            case 0x02:
                printLine("Nono, it's user mode.");
                return 0;
            // case 0x05:
            //     break;
            // case 0x07:
            //     break;
            case 0x08:
            case 0x09: {
                uint64 volatile ret = handle_syscall(a0, a1, a2, a3);
                // ((uint64*)sp)[31] += 4; // sepc += 4;
                // ((uint64*)sp)[23] = ret; // a0 = ret;
                Frame* frame = (Frame*)sp;
                frame->sepc += 4;
                frame->a[0] = ret;
                
                return ret;
            }
            default:
                // Ne sme se ovde doci.
                return 0UL;
        }
    }
}

uint64 handle_syscall(uint64 a0, uint64 a1, uint64 a2, uint64 a3) {
    
    switch (a0) {
        case 0x01:
            return (uint64)MemoryAllocator::mem_alloc((size_t)a1 * MEM_BLOCK_SIZE);
        case 0x02:
            return (uint64)MemoryAllocator::mem_free((void*)a1);
        case 0x11:
            return (uint64)TCB::create((void(*)(void*))a2, (void*)a3);
        case 0x12:
            return (uint64)TCB::exit();
        case 0x13:
            TCB::dispatch();
            return 0UL;
        default:
            return 0UL;
    }
}