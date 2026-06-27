#include "../h/Controller.hpp"


uint64 Controller::read_scause() {
    uint64 scause;
    asm volatile("csrr %0, scause" : "=r"(scause));
    return scause;
}

void Controller::write_scause(uint64 scause) {
    asm volatile("csrw scause, %0" : : "r"(scause));
}

uint64 Controller::read_stvec() {
    uint64 stvec;
    asm volatile("csrr %0, stvec" : "=r"(stvec));
    return stvec;
}

void Controller::write_stvec(uint64 stvec) {
    asm volatile("csrw stvec, %0" : : "r"(stvec));
}

uint64 Controller::read_sepc() {
    uint64 sepc;
    asm volatile("csrr %0, sepc" : "=r"(sepc));
    return sepc;
}

void Controller::write_sepc(uint64 sepc) {
    asm volatile("csrw sepc, %0" : : "r"(sepc));
}

uint64 Controller::read_stval() {
    uint64 stval;
    asm volatile("csrr %0, stval" : "=r"(stval));
    return stval;
}

void Controller::write_stval(uint64 stval) {
    asm volatile("csrw stval, %0" : : "r"(stval));
}

uint64 Controller::read_sstatus() {
    uint64 sstatus;
    asm volatile("csrr %0, sstatus" : "=r"(sstatus));
    return sstatus;
}

void Controller::write_sstatus(uint64 sstatus) {
    asm volatile("csrw sstatus, %0" : : "r"(sstatus));
}

void mask_set_sstatus(uint64 mask) {

}

void mask_clear_sstatus(uint64 mask) {
     
}

void Controller::exit() {
    asm volatile(
        "li t0, 0x100000\n"
        "li t1, 0x5555\n"
        "sw t1, 0(t0)\n"
    );
}