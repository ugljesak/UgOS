#include "../h/utils.hpp"
#include "../h/testMemoryAllocator.hpp"
#include "../h/testSystemThread.hpp"
#include "../h/Controller.hpp"

void userMain(void* arg) {
    printLine("wow");
    //printLine("kurcina.");
    //__putc('$');
    //testMemoryAllocator();
    //printLine("Finished memory testing.");
    
    System_Mode_test();
    printLine("Finished threads testing.");

}