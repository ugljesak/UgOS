#include "../h/MemoryAllocator.hpp"
#include "../h/TCB.hpp"
#include "../h/utils.hpp"
#include "../h/Controller.hpp"
#include "../lib/console.h"


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
uint64 handle_timer();

uint64 handle_trap(uint64 cause, uint64 sp, uint64 a0, uint64 a1, uint64 a2, uint64 a3) { 
    //printValue("TRAP cause", cause);
    Frame* frame = (Frame*)sp;
    
    if(cause & (1UL << 63)) {
        //asm volatile("csrc sstatus, 0x2");
        cause &= ~(1UL << 63);
        switch(cause) {
            case 0x01:
                handle_timer();
                break;
            case 0x09:
                console_handler();
                break;
            default:
                break;
        }
        return 0UL;
    }
    else{
        // Unutrasnji prekid
        switch(cause) {
            case 0x02:
                printValue("ILLEGAL INST AT SEPC", frame->sepc);
                Controller::exit();
                break;
            case 0x05:
                frame->sepc += 4;
                printValue("ILLEGAL READ AT SEPC", frame->sepc);
                Controller::exit();
                break;
            case 0x07:
                printValue("ILLEGAL WRITE AT SEPC", frame->sepc);
                Controller::exit();
                break;
            case 0x08:
            case 0x09: {
                uint64 volatile ret = handle_syscall(a0, a1, a2, a3);
                // ((uint64*)sp)[31] += 4; // sepc += 4;
                // ((uint64*)sp)[23] = ret; // a0 = ret;
                frame->sepc += 4;
                frame->a[0] = ret;
                
                return ret;
            }
            default:
                printValue("UNKNOWN CODE", cause);
                printValue("SEPC =", frame->sepc);
                Controller::exit();
                
                return 0UL;
        }
    }
    return 0UL;
}

uint64 handle_timer() {
    TCB::incTimeCounter();
    // printLine("Timer okinut.");
    if(TCB::getTimeCounter() >= TCB::running->getTimeSlice()) {
        
        uint64 sepc = Controller::read_sepc();
        uint64 sstatus = Controller::read_sstatus();
        
        TCB::resetTimeCounter();
        TCB::dispatch();
        
        Controller::write_sstatus(sstatus); 
        Controller::write_sepc(sepc);
    }
    Controller::mask_clear_sip(Controller::SIP_SSIP);
    return 0;
}

uint64 handle_syscall(uint64 a0, uint64 a1, uint64 a2, uint64 a3) {
    //printValue("Kod op", a0);
    switch (a0) {
        case 0x01:
            return (uint64)MemoryAllocator::mem_alloc((size_t)a1 * MEM_BLOCK_SIZE);
        case 0x02:
            return (uint64)MemoryAllocator::mem_free((void*)a1);
        case 0x11:
            return (uint64)TCB::create((void(*)(void*))a2, (void*)a3);
        case 0x12:
            TCB::running->setFinished();
            TCB::dispatch();
            return 0UL;
            
        case 0x13: {
            uint64 sepc = Controller::read_sepc();
            uint64 sstatus = Controller::read_sstatus();
            
            TCB::dispatch();
            
            Controller::write_sepc(sepc);
            Controller::write_sstatus(sstatus); 
            return 0UL;
        }
        default:
            return 0UL;
    }
}