#ifndef UGOS_CONTROLLER_H
#define UGOS_CONTROLLER_H

#include "../lib/hw.h"

class Controller {
public:

    static uint64 read_scause();
    static void write_scause(uint64 scause);

    static uint64 read_stvec();
    static void write_stvec(uint64 stvec);

    static uint64 read_sepc();
    static void write_sepc(uint64 sepc);

    static uint64 read_stval();
    static void write_stval(uint64 scause);

    enum BitmaskSIP {
        SIP_SSIE = (1 << 1),
        SIP_STIE = (1 << 5),
        SIP_SEIE = (1 << 9)
    };

    static uint64 read_sstatus();
    static void write_sstatus(uint64 scause);
    static void mask_set_sstatus(uint64 mask);
    static void mask_clear_sstatus(uint64 mask);

    static void exit();

};

#endif