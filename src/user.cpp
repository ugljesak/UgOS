#include "../h/utils.hpp"
#include "../h/testMemoryAllocator.hpp"
#include "../h/testSystemThread.hpp"
#include "../h/Controller.hpp"

void userMain(void*) {
    printLine("wow");
    //printLine("kurcina.");
    //__putc('$');
    //testMemoryAllocator();
    //printLine("Finished memory testing.");
    
    Threads_C_API_test();
    printLine("Finished threads testing.");

}