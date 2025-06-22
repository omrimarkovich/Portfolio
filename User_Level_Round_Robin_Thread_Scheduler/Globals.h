//
// Created by omri on 6/20/2025.
//

#ifndef PRATFOLIO_GLOBALS_H
#define PRATFOLIO_GLOBALS_H

/************************************
 * Standard Library Includes
 ************************************/
#include <iostream>
#include <cstdlib>
#include <csignal>
#include <sys/time.h>
#include <setjmp.h>
#include <unistd.h>
#include <queue>
#include <unordered_map>
#include <unordered_set>

/************************************
 * Macro Definitions
 ************************************/
#define MAX_THREAD_NUM 100                   // Maximum number of threads
#define STACK_SIZE 100000                    // Stack size per thread in bytes (TODO: adjust before submission 4096 \ 100000))
#define JB_SP 6                              // Index of Stack Pointer in jmp buffer
#define JB_PC 7                              // Index of Program Counter in jmp buffer
#define SECOND 1000000                       // Microseconds in one second
#define FAILURE -1
#define SUCCESS 0

/************************************
 * Error Handling Macros
 ************************************/
#define FUNCTION_ERROR(msg, return_arg) \
    { std::cerr << "thread library error: " << msg << std::endl; return return_arg; }

#define SYSTEM_ERROR(msg, handler_func) \
    { std::cerr << "system error: " << msg << std::endl; handler_func; }

/************************************
 * Default Identifiers
 ************************************/
#define MAIN_THREAD_ID 0

/************************************
 * Custom Types
 ************************************/
typedef void (*thread_entry_point)(void);    // Function pointer type for thread entry
typedef unsigned long address_t;             // Memory address type

/************************************
 * Enums
 ************************************/
typedef enum SwitchEvent {
    RESUME_EVENT,
    TERMINATE_EVENT,
    BLOCK_EVENT,
    SLEEP_EVENT,
    PREEMPT_EVENT
} SwitchEvent_EVENT;

typedef enum exit_stsus {
    CLEAR,
    ERROR
} exit_stsus;

/************************************
 *  Global Variable Declarations
 * (Definitions must go in Globals.cpp)
 ************************************/
extern size_t total_quantums;                         // Total number of quantums since init
extern unsigned int current_thread_id;                // ID of the currently running thread
extern size_t init_cuantom_usecs;                     // Length of one quantum in microseconds

extern struct sigaction sa;                           // Signal handler configuration
extern struct itimerval timer;                        // Timer for virtual clock
extern sigset_t sig_set;                              // Signal set for blocking

extern std::unordered_map<int, int> sleep_map;        // Map: tid → remaining sleep time
extern std::unordered_set<int> deleted_threads;       // Set of logically deleted threads
extern std::queue<unsigned int> ready_queue;          // Queue of threads in READY state

#endif // PRATFOLIO_GLOBALS_H
