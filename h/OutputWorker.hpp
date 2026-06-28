#ifndef UGOS_OUTPUT_WORKER_HPP
#define UGOS_OUTPUT_WORKER_HPP

#include "../lib/hw.h"
#include "ConsoleBuffer.hpp"

class OutputWorker {
public:
    static void init();
    static int put(char c);

    static ConsoleBuffer* outputBuffer;

private:
    static void run(void*);
};

#endif // UGOS_OUTPUT_WORKER_HPP