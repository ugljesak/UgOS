#include "../h/syscall_c.hpp"
#include "../lib/console.h"
#include "../h/utils.hpp"

// forward deklaracije
void threadA(void*);
void threadB(void*);
void producer(void*);
void consumer(void*);

sem_t sem;

void threadA(void* arg) {
    sem_t* done = (sem_t*)arg;
    for(int i = 0; i < 5; i++) {
        printString("A\n");
        thread_dispatch();
    }
    sem_signal(*done);
    thread_exit();
}

void threadB(void* arg) {
    sem_t* done = (sem_t*)arg;
    for(int i = 0; i < 5; i++) {
        printString("B\n");
        thread_dispatch();
    }
    sem_signal(*done);
    thread_exit();
}

void producer(void* arg) {
    sem_t* done = (sem_t*)arg;
    printString("Producer radi...\n");
    thread_dispatch();
    printString("Producer signalizira\n");
    sem_signal(sem);
    sem_signal(*done);
    thread_exit();
}

void consumer(void* arg) {
    sem_t* done = (sem_t*)arg;
    printString("Consumer ceka\n");
    sem_wait(sem);
    printString("Consumer probuden!\n");
    sem_signal(*done);
    thread_exit();
}

void testFully() {
    sem_t done;
    sem_open(&done, 0);
    sem_open(&sem, 0);

    thread_t a, b, p, c;
    thread_create(&a, threadA, (void*)&done);
    thread_create(&b, threadB, (void*)&done);
    thread_create(&p, producer, (void*)&done);
    thread_create(&c, consumer, (void*)&done);

    sem_wait_n(done, 4);

    sem_close(done);
    sem_close(sem);
}