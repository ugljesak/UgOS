#ifndef UGOS_SYSCALL_CPP_HPP
#define UGOS_SYSCALL_CPP_HPP

#include "syscall_c.hpp"

class Thread {
public:

    Thread (void (*body)(void*), void* arg);
    virtual ~Thread ();
    int start ();

    static void dispatch ();
    static int sleep (time_t);

protected:
    Thread ();
    virtual void run () {}

private:
    void (*body)(void*);
    void* arg;
    thread_t myHandle;

    static void bodyWrapper(void* arg);
};

class Semaphore {
public:

    Semaphore (unsigned init = 1);
    virtual ~Semaphore ();

    int wait ();
    int signal ();

private:
    sem_t myHandle;
};

class Console {
public:
    static char getc ();
    static void putc (char);
};

class PeriodicThread : public Thread {
public:
    void terminate();

protected:
    PeriodicThread(time_t period);
    virtual void periodicActivation() {}

private:
    time_t period;
    bool terminated = false;

    void run() override {
        while(!terminated) {
            periodicActivation();
            time_sleep(period);
        }
        thread_exit();
    }
};


#endif //UGOS_SYSCALL_CPP_HPP
