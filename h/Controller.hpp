#ifndef UGOS_CONTROLLER_H
#define UGOS_CONTROLLER_H

#include "../lib/hw.h"

class Controller {
public:
    static void sReturn();

    inline static uint64 read_scause() {
        uint64 scause;
        asm volatile("csrr %0, scause" : "=r"(scause));
        return scause;
    }
    inline static void write_scause(uint64 scause) {
       asm volatile("csrw scause, %0" : : "r"(scause));
    }

    inline static uint64 read_stvec() {
        uint64 stvec;
        asm volatile("csrr %0, stvec" : "=r"(stvec));
        return stvec;
    }
    inline static void write_stvec(uint64 stvec) {
       asm volatile("csrw stvec, %0" : : "r"(stvec));
    }

    inline static uint64 read_sepc() {
        uint64 sepc;
        asm volatile("csrr %0, sepc" : "=r"(sepc));
        return sepc;
    }
    inline static void write_sepc(uint64 sepc) {
        asm volatile("csrw sepc, %0" : : "r"(sepc));
    }


    inline static uint64 read_stval() {
        uint64 stval;
        asm volatile("csrr %0, stval" : "=r"(stval));
        return stval;
    }
    inline static void write_stval(uint64 stval) {
        asm volatile("csrw stval, %0" : : "r"(stval));
    }

    enum BitmaskSIP {
        SIP_SSIP = (1 << 1),
        SIP_STIP = (1 << 5),
        SIP_SEIP = (1 << 9)
    };

    inline static void mask_set_sip(uint64 mask) {
        asm volatile ("csrs sip, %0" : : "r"(mask));
    }
    inline static void mask_clear_sip(uint64 mask) {
        asm volatile ("csrc sip, %0" : : "r"(mask));
    }

    enum BitmaskSstatus
    {
        SSTATUS_SIE = (1 << 1),
        SSTATUS_SPIE = (1 << 5),
        SSTATUS_SPP = (1 << 8),
    };

    inline static uint64 read_sstatus() {
        uint64 sstatus;
        asm volatile("csrr %0, sstatus" : "=r"(sstatus));
        return sstatus;
    }
    inline static void write_sstatus(uint64 sstatus) {
       asm volatile("csrw sstatus, %0" : : "r"(sstatus));
    }

    
    inline static void mask_set_sstatus(uint64 mask) {
        asm volatile ("csrs sstatus, %0" : : "r"(mask));
    }

    inline static void mask_clear_sstatus(uint64 mask) {
        asm volatile ("csrc sstatus, %0" : : "r"(mask));
    }
    static void exit();

};

#endif