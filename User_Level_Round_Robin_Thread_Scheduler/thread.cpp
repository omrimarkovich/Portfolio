//
// Created by ommri on 6/19/2025.
//
#include "thread.h"

Thread::Thread(int tid, ThreadStatus state, int quantum_count, char *stack,
       thread_entry_point entry_point) : tid(tid), status(state), stack(stack), quantum_count(quantum_count) {
    pc = (address_t) entry_point;
    if (stack != nullptr) {
        sp = (address_t) stack + STACK_SIZE - sizeof(address_t);
        pc = (address_t) entry_point;
    }

    if (sigsetjmp(env, 1) == 0) {
        (env->__jmpbuf)[JB_SP] = translate_address(sp);
        (env->__jmpbuf)[JB_PC] = translate_address(pc);
        sigemptyset(&env->__saved_mask);
    }
}

/* A translation is required when using an address of a variable.
Use this as a black box in your code. */
address_t Thread::translate_address(address_t addr) {
    address_t ret;
    asm volatile("xor    %%fs:0x30,%0\n"
                 "rol    $0x11,%0\n"
            : "=g" (ret)
            : "0" (addr));
    return ret;
}

Thread::~Thread(){
    if (stack != nullptr) {
        delete[] stack;
        stack = nullptr;
    }
    delete threads_array[tid];
    threads_array[tid] = nullptr;
}

