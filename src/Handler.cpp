#include "../lib/console.h"
#include "../h/Handler.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/TCB.hpp"
#include "../h/utils.hpp"
#include "../h/Controller.hpp"
#include "../h/_Sem.hpp"
#include "../h/Scheduler.hpp"
#include "../h/OutputWorker.hpp"
#include "../h/syscall_c.hpp"
#include "../h/InputWorker.hpp"

uint64 Handler::handle_trap(uint64 cause, uint64 sp, uint64 a0, uint64 a1, uint64 a2, uint64 a3) { 
    Frame* frame = (Frame*)sp;
    //printValue("TRAP CAUSE", cause);

    if(cause & (1UL << 63)) {
        cause &= ~(1UL << 63);
        switch(cause) {
            case 0x01:
                handle_timer();
                break;
            case 0x09:
                handle_console();
                break;
            default:
                printValue("Illegal cause", cause);
                break;
        }
        return 0UL;
    }
    else{
        // Unutrasnji prekid
        switch(cause) {
            case 0x02:
                printValue("ILLEGAL INSTRUCTION AT SEPC", frame->sepc);
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
                
                frame->sepc += 4;
                frame->a[0] = ret;
                
                return ret;
            }
            default:
                printValue("UNKNOWN CODE", cause);
                printValue("SEPC", frame->sepc);
                printValue("SP", sp);
                Controller::exit();
                
                return 0UL;
        }
    }
    return 0UL;
}

uint64 Handler::handle_timer() {
    TCB::timeCounter++;
    // printLine("Timer okinut.");
    Controller::mask_clear_sip(Controller::SIP_SSIP);

    Scheduler::sleepTick();
    
    if(TCB::timeCounter >= TCB::running->getTimeSlice()) {
        
        uint64 sepc = Controller::read_sepc();
        uint64 sstatus = Controller::read_sstatus();
        
        TCB::timeCounter = 0;
        TCB::dispatch();
        
        Controller::write_sstatus(sstatus); 
        Controller::write_sepc(sepc);
    }
    return 0;
}

uint64 Handler::handle_console() {
    int irq = plic_claim();

    if(irq == CONSOLE_IRQ) {
        volatile uint8* consoleStatus = (volatile uint8*)CONSOLE_STATUS;
        volatile uint8* consoleRXData = (volatile uint8*)CONSOLE_RX_DATA;

        while(*consoleStatus & CONSOLE_RX_STATUS_BIT) {
            char c = *consoleRXData;
            InputWorker::inputBuffer->putFromISR(c);
        }
        plic_complete(CONSOLE_IRQ);
    }
    return 0UL;
}

uint64 Handler::handle_syscall(uint64 a0, uint64 a1, uint64 a2, uint64 a3) {
    //printValue("Kod op", a0);
    switch (a0) {
        case 0x01:
            return (uint64)MemoryAllocator::mem_alloc((size_t)a1 * MEM_BLOCK_SIZE);
        case 0x02:
            return (uint64)MemoryAllocator::mem_free((void*)a1);
        case 0x11:
            return (uint64)TCB::create((void(*)(void*))a1, (void*)a2);
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
        case 0x21:
            return (uint64)(new _Sem((unsigned)a1));
        case 0x22:
            return (uint64)((_Sem*)a1)->close();
        case 0x23:
            return (uint64)((_Sem*)a1)->wait();
        case 0x24:
            return (uint64)((_Sem*)a1)->signal();
        case 0x25:
            return (uint64)((_Sem*)a1)->waitN((unsigned)a2);
        case 0x26:
            return (uint64)((_Sem*)a1)->signalN((unsigned)a2);
        case 0x31:
            return (uint64)(Scheduler::sleep(TCB::running, a1));
        case 0x41:
            return (uint64)InputWorker::get();
        case 0x42:{
            return (uint64)(OutputWorker::put(a1));
        }
        default:
            printLine("Invalid ecall operation code.");
            return 0UL;
    }
}