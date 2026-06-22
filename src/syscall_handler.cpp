#include "../h/MemoryAllocator.hpp"

extern "C" uint64 handle_syscall(uint64 a0, uint64 a1, uint64 a2, uint64 a3);

extern "C" uint64 handle_trap(uint64 cause, uint64 a0, uint64 a1, uint64 a2, uint64 a3) { 
    
    if(cause & (1UL << 63)) {
        // Spoljasnji prekid
        return 0UL;
    }
    else{
        // Unutrasnji prekid
        switch(cause) {
            // case 0x02:
            //     break;
            // case 0x05:
            //     break;
            // case 0x07:
            //     break;
            // case 0x08:
            //     break;    
            case 0x09:
                return handle_syscall(a0, a1, a2, a3);
                break;
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
        //case 0x11:
            //...
        default:
            return 0UL;
    }
}