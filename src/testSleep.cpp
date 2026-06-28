#include "../h/_Sem.hpp"
#include "../h/utils.hpp"
#include "../h/syscall_c.hpp"


struct Args {
    uint64 id;
    sem_t sem;
};

void sleeper(void* arg) {
    Args* args = (Args*)(arg);
    
    printValue("Thread usao", args->id);
    time_sleep(args->id * 10);
    printValue("Thread se probudio", args->id);
    sem_signal(args->sem);
    thread_exit();
}

void sleepTest() {
    sem_t done;
    sem_open(&done, 0);
    
    thread_t threads[5];
    Args** args = (Args**)mem_alloc(5 * sizeof(Args*));

        
    for(int i = 4; i >= 0; i--) {
        args[i] = (Args*)mem_alloc(sizeof(Args));
        args[i]->id = (uint64)i+1;
        args[i]->sem = done;
        thread_create(&threads[i], sleeper, (void*)args[i]);
    }
    
    sem_wait_n(done, 5);
    //sem_wait(done);sem_wait(done);sem_wait(done);sem_wait(done);sem_wait(done);
    printString("Sve niti zavrsene!\n");
    sem_close(done);
    printLine("Unistio semafor.");
}