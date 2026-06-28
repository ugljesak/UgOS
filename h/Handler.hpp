#include "../lib/hw.h"


class Handler {
public:
    static uint64 handle_trap(uint64 cause, uint64 sp, uint64 a0, uint64 a1, uint64 a2, uint64 a3);

private:
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
    static uint64 handle_timer();
    static uint64 handle_console();
    static uint64 handle_syscall(uint64 a0, uint64 a1, uint64 a2, uint64 a3);

};