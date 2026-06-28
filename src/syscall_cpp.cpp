#include "../h/syscall_cpp.hpp"

// ----------------------------------------
// Thread
// ----------------------------------------

Thread::Thread(void (*body)(void*), void* arg) : body(body), arg(arg), myHandle(nullptr) {}

Thread::Thread() : body(nullptr), arg(nullptr), myHandle(nullptr) {}

Thread::~Thread() {
    mem_free(myHandle);
}

void Thread::bodyWrapper(void* arg) {
    Thread* t = (Thread*)arg;
    t->run();
}

int Thread::start() {
    if(myHandle) return -1;
    if(body) {
        return thread_create(&myHandle, body, arg);
    } else {
        return thread_create(&myHandle, bodyWrapper, this);
    }
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::sleep(time_t t) {
    return time_sleep(t);
}

// ----------------------------------------
// Semaphore
// ----------------------------------------

Semaphore::Semaphore(unsigned init) {
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

// ----------------------------------------
// Console
// ----------------------------------------

char Console::getc() {
    return ::getc();
}

void Console::putc(char c) {
    ::putc(c);
}

// ----------------------------------------
// Periodic Thread
// ----------------------------------------

PeriodicThread::PeriodicThread(time_t period) 
    : period(period), terminated(false) {}

void PeriodicThread::terminate() {
    terminated = true;
}