#include "../h/InputWorker.hpp"
#include "../h/utils.hpp"


ConsoleBuffer* InputWorker::inputBuffer = nullptr;

void InputWorker::init() {
    inputBuffer = new ConsoleBuffer();
    *(volatile uint32*)(0x0C000028) = 1; // priority za IRQ 10
    *(volatile uint32*)(0x0C002080) = (1 << 10); // enable IRQ 10 za S-mode
    *(volatile uint32*)(0x0C201000) = 0; // threshold = 0
}


char InputWorker::get() {
    return inputBuffer->get();
}