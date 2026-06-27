#include "../h/utils.hpp"
#include "../h/testMemoryAllocator.hpp"
#include "../h/testSystemThread.hpp"
#include "../h/System_Mode_test.hpp"
#include "../h/Controller.hpp"

void userMain(void*) {
    printLine("wow");
    //printLine("kurcina.");
    //__putc('$');
    //testMemoryAllocator();
    //printLine("Finished memory testing.");
    //Controller::mask_set_sstatus(Controller::SSTATUS_SIE);
    
    //Threads_C_API_test();
    System_Mode_test();
    printLine("Finished threads testing.");

}