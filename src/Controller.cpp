#include "../h/Controller.hpp"


void Controller::sReturn(){
    asm volatile("csrw sepc, ra");
    asm volatile("sret");
}


void Controller::exit() {
    asm volatile(
        "li t0, 0x100000\n"
        "li t1, 0x5555\n"
        "sw t1, 0(t0)\n"
    );
}