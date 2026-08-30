<h1 align="center">UgOS</h1>

<p align="center">
  A small preemptive multithreaded kernel for RISC-V, written from scratch in C++.
</p>

<p align="center">
  <img alt="arch" src="https://img.shields.io/badge/arch-RISC--V%20rv64ima-8A2BE2">
  <img alt="lang" src="https://img.shields.io/badge/language-C%2B%2B11%20%2F%20asm-00599C">
  <img alt="target" src="https://img.shields.io/badge/target-QEMU%20virt-red">
  <img alt="libc" src="https://img.shields.io/badge/freestanding-no%20libc-lightgrey">
</p>

---

## What is it

UgOS is a bare-metal educational operating system kernel that boots on a QEMU
`virt` RISC-V machine with no bootloader, no standard library and no host OS
underneath it. It brings up its own heap, trap handling, threads, scheduler,
synchronization primitives and a buffered console — everything from the first
instruction up.

## Features

| | |
|---|---|
| **Threads** | Preemptive round-robin scheduling on a timer interrupt, with explicit `dispatch()` for cooperative yields |
| **Context switching** | Hand-written RISC-V assembly (`yield.S`, `trap_entry.S`) saving the full trap frame |
| **Privilege modes** | Kernel runs in supervisor mode, user threads in user mode; crossing happens only through `ecall` |
| **System calls** | Trap-based ABI — allocation, threads, semaphores, sleep and console I/O dispatched by opcode |
| **Memory** | Custom first-fit heap allocator with block splitting and neighbour coalescing |
| **Synchronization** | Counting semaphores with blocked-thread queues, plus n-ary `wait`/`signal` and an ISR-safe signal |
| **Sleeping** | Relative-delta sleep queue advanced by the timer, so a tick only touches the head |
| **Console** | Interrupt-driven input and output workers over bounded ring buffers (producer/consumer) |
| **Periodic threads** | `PeriodicThread` base class for tasks that wake on a fixed period |

## API

Two interchangeable layers over the same syscalls — a C API and a C++ one.

```cpp
// C
thread_t t;
thread_create(&t, workerBody, nullptr);
sem_t s; sem_open(&s, 1);
sem_wait(s); /* ... */ sem_signal(s);
time_sleep(50);

// C++
class Worker : public Thread { void run() override { /* ... */ } };
Semaphore mutex(1);
Thread::dispatch();
```

## Build & run

Requires a `riscv64` GCC toolchain and `qemu-system-riscv64`.

```bash
make          # build the kernel image
make qemu     # boot it
make qemu-gdb # boot halted, waiting for a debugger
```

On boot the kernel starts a user thread that prompts for a test number `[1-7]`,
covering both API layers, synchronous and asynchronous context switching,
producer/consumer, sleeping and user-mode enforcement.

## Layout

```
h/      headers — TCB, Scheduler, Handler, MemoryAllocator, semaphores, console
src/    kernel and test implementation; .S files hold the context-switch core
lib/    prebuilt hardware/console support libraries
kernel.ld, Makefile
```

## License

MIT — parts derive from the xv6 teaching kernel (MIT PDOS). See [LICENSE](LICENSE).
