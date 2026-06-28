#ifndef UGOS_INPUT_WORKER_HPP
#define UGOS_INPUT_WORKER_HPP

#include "../lib/hw.h"
#include "ConsoleBuffer.hpp"

class InputWorker {
public:
    static void init();
    static char get();

    static ConsoleBuffer* inputBuffer;

private:
    static void run(void*);
};

#endif // UGOS_INPUT_WORKER_HPP