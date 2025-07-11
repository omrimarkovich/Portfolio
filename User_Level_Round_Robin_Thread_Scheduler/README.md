# User-Level Threads Library (uthreads)

## Overview
This project implements a user-level threading library in C++ that supports basic thread management functionalities such as creating, terminating, blocking, resuming, and sleeping threads. The library uses a round-robin scheduler with preemption via timer interrupts, enabling cooperative multitasking within a single process without relying on OS kernel threads.

## Project Motivation
Modern applications often require concurrent execution of multiple tasks to improve responsiveness and efficiency. While kernel-level threads provide this capability, user-level threads can offer lighter-weight and more controllable concurrency with reduced context-switch overhead.

The goal of this project was to design and implement a lightweight user-level threading library from scratch, managing its own scheduling and context switching using setjmp/longjmp and signal handling, while also supporting thread synchronization primitives.

## Key Features
Thread creation and termination with independent stacks.

Round-robin scheduling using a timer interrupt (SIGVTALRM) for preemption.

Thread states: RUNNING, READY, BLOCKED, SLEEP, and combinations.

Thread blocking and resuming functionality.

Thread sleeping for a specified number of quantums.

Safe signal handling and context switching.

Management of thread lifecycle with proper cleanup of resources.

## Approach and Implementation Details

### Design
Each thread is represented by a Thread object that holds its stack, program counter (PC), stack pointer (SP), and execution context via sigjmp_buf.

A global threads array holds all active thread objects, indexed by thread ID.

The scheduler uses a ready queue to keep track of runnable threads.

Signals (SIGVTALRM) are used to trigger preemption at fixed quantum intervals.

Context switches save and restore thread states using sigsetjmp and siglongjmp.

### Modules
Thread Management (thread.h / thread.cpp): Defines the Thread class, manages stacks, PC/SP translation, and context storage.

Scheduler (scheduler.h / scheduler.cpp): Implements the round-robin scheduling logic, event handling (preempt, block, sleep, terminate).

Signal Handling (signals.h / signals.cpp): Sets up timer signals and handles signal masks for atomic operations.

Utilities (utils.h / utils.cpp): Provides helper functions such as validating thread existence, deleting threads, and managing the ready queue.

Globals (Globals.h): Contains constants, macros, global variables, and type definitions.

### Challenges and Solutions
Context switching: Implemented using sigsetjmp/siglongjmp with address translation to manage stack pointers and program counters safely.

Signal safety: Carefully block signals during critical operations to prevent race conditions.

Thread sleeping: Managed using a map of sleeping threads with remaining quantum counts; scheduler wakes them appropriately.

Resource management: Deferred thread deletion via a deleted_threads set to avoid deleting active threads during context switches.

## How to Use
Initialize the library: Call uthread_init(quantum_usecs) with desired quantum length in microseconds.

Create threads: Use uthread_spawn(thread_entry_point) to create new threads.

Thread control: Use uthread_block, uthread_resume, uthread_sleep, and uthread_terminate as needed.

Query threads: Retrieve information like current thread ID and quantums with uthread_get_tid and uthread_get_quantums.

## Summary
This project demonstrates how to build a user-level threading library from the ground up, handling the complexities of context switching, scheduling, and synchronization. It showcases low-level systems programming techniques in C++ and Unix signal handling, laying a foundation for understanding concurrency beyond kernel threads.

