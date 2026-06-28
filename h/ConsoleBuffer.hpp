#ifndef UGOS_CONSOLE_BUFFER_HPP
#define UGOS_CONSOLE_BUFFER_HPP

#include "../lib/hw.h"
#include "_Sem.hpp"
#include "../h/utils.hpp"

#define BUFFER_SIZE 4096

class ConsoleBuffer {
public:
    ConsoleBuffer() : 
        head(0), tail(0),
        notEmpty((new _Sem(0))),
        // notFull(new _Sem(1)),
        mutex(new _Sem(1))
    {}

    int put(char c) {
        // notFull->wait();
        mutex->wait();
        if((tail + 1) % BUFFER_SIZE == head) {
            mutex->signal();
            return -1;
        }
        buf[tail] = c;
        tail = (tail + 1) % BUFFER_SIZE;
        mutex->signal();
        notEmpty->signal();
        TCB::dispatch();
        return 0;
    }
    
    int putFromISR(char c) {
        if((tail + 1) % BUFFER_SIZE == head) return -1;
        buf[tail] = c;
        tail = (tail + 1) % BUFFER_SIZE;
        notEmpty->signalFromISR();
        return 0;
    }

    char get() {
        notEmpty->wait();
        mutex->wait();
        char c = buf[head];
        head = (head + 1) % BUFFER_SIZE;
        mutex->signal();
        // notFull->signal();
        return c;
    }

    inline bool isEmpty() { return head == tail; }

private:
    char buf[BUFFER_SIZE];
    int head, tail;
    _Sem* notEmpty;
    //_Sem* notFull;
    _Sem* mutex;
};

#endif // UGOS_CONSOLE_BUFFER_HPP