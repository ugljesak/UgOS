#include "../h/utils.hpp"
#include "../h/testMemoryAllocator.hpp"
#include "../h/testSystemThread.hpp"
#include "../h/System_Mode_test.hpp"
#include "../h/Controller.hpp"
#include "../h/buffer.hpp"
#include "../h/syscall_cpp.hpp"

class Blinker : public PeriodicThread {
public:
    Blinker() : PeriodicThread(5) {}
protected:
    void periodicActivation() override {
        printString("Blink!\n");
    }
};

extern void testFully();
extern void sleepTest();
void userMain1(void*) {
    printLine("Entered user mode.");
    //printLine("kurcina.");
    //__putc('$');
    //testMemoryAllocator();
    //printLine("Finished memory testing.");
    printString("Hello from printString\n");
    
    // tvoj putc
    const char* msg = "Hello from my putc\n";
    for(int i = 0; msg[i] != '\0'; i++) {
        putc(msg[i]);
    }
    printString("Start\n");
    
    Blinker* b = new Blinker();
    b->start();
    
    Thread::sleep(25);  // cekaj 5 blink-ova
    
    b->terminate();
    
    Thread::sleep(10);  // daj mu vremena da se ugasi
    
    printString("Kraj\n");

    // printString("Ukucaj nesto:\n");
    // while(true) {
    //     char c = getc();
    //     putc(c);  // echo nazad
    //     if(c == 'q') break;
    // }
    // printString("\nGotovo!\n");
    //sleepTest();

    //Threads_C_API_test();
    //System_Mode_test();
    printLine("Finished user testing.");

}