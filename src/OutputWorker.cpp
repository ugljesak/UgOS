#include "../h/OutputWorker.hpp"
#include "../h/utils.hpp"


ConsoleBuffer* OutputWorker::outputBuffer = nullptr;

void OutputWorker::init() {
    outputBuffer = new ConsoleBuffer();
    TCB::createDaemon(run, nullptr);
}

void OutputWorker::run(void*) {
    printString("RUN\n");

    while(true) {
        
        char c = outputBuffer->get();
        
        volatile uint8* consoleStatus = (volatile uint8*)CONSOLE_STATUS;
        volatile uint8* consoleTXData = (volatile uint8*)CONSOLE_TX_DATA;
        
        while(!(*consoleStatus & CONSOLE_TX_STATUS_BIT));
        *consoleTXData = (uint8)c;
    }
}

int OutputWorker::put(char c) {
    return outputBuffer->put(c);
}