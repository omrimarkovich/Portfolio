//
// Created by ommri on 6/19/2025.
//

#ifndef PRATFOLIO_THREAD_H
#define PRATFOLIO_THREAD_H

#include <setjmp.h>
#include "Globals.h"
#include <signal.h>
#include <cstring>


// Type alias for memory addresses
typedef unsigned long address_t;

// Type for the thread entry point function
typedef void (*thread_entry_point)(void);

// Enum to represent the state of a thread
typedef enum ThreadStatus {
    BLOCKED,
    SLEEP,
    BLOKED_AND_SLEEP,
    READY,
    RUNNING
} ThreadStatus;



class Thread {
private:
    int tid;
    ThreadStatus status;
    char *stack;
    address_t pc;
    address_t sp;
    int quantum_count;
    sigjmp_buf env;

public:
    /**
     * @brief Constructs a new Thread object.
     */
    Thread(int tid, ThreadStatus state, int quantum_count, char *stack,
           thread_entry_point entry_point);

    /**
     * @brief Destroys the Thread object and frees associated resources.
     */
    ~Thread();

    /**
     * @brief Translates a logical address to a physical address.
     */
    static address_t translate_address(address_t addr);

    // --- Getters ---
    int getTid() const { return tid; }
    ThreadStatus& getStatus()  { return status; }
    char* getStack() const { return stack; }
    address_t getPc() const { return pc; }
    address_t getSp() const { return sp; }
    int getQuantumCount() const { return quantum_count; }
    sigjmp_buf& getEnv() { return env; }

    // --- Setters ---
    void setTid(int new_tid) { tid = new_tid; }
    void setStatus(ThreadStatus new_status) { status = new_status; }
    void setStack(char* new_stack) { stack = new_stack; }
    void setPc(address_t new_pc) { pc = new_pc; }
    void setSp(address_t new_sp) { sp = new_sp; }
    void setQuantumCount() { ++quantum_count; }
    void setQuantumCount(int new_quantum_count) { quantum_count = new_quantum_count; }
    void setEnv(const sigjmp_buf& new_env) { std::memcpy(env, new_env, sizeof(sigjmp_buf)); }

};

Thread *threads_array[MAX_THREAD_NUM] = {nullptr}; // Array of threads


#endif //PRATFOLIO_THREAD_H
