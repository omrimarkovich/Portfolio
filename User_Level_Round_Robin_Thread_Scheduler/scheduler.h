//
// Created by ommri on 6/22/2025.
//

#ifndef PRATFOLIO_SCHEDULER_H
#define PRATFOLIO_SCHEDULER_H

#include "thread.h"
#include "signals.h"
#include "thread.h"
#include "utils.h"

#include <queue>
#include <unordered_map>
/**
 * @brief Handles a context switch event and manages thread transitions.
 *
 * Depending on the provided event type, this function saves the current
 * thread's state using `sigsetjmp`, updates its status, and either:
 *   - Preempts it (adds it back to the ready queue),
 *   - Blocks it,
 *   - Puts it to sleep,
 *   - Terminates it.
 *
 * The function then restores the next thread’s context using `siglongjmp`.
 *
 * @param event The type of switch event (e.g., PREEMPT_EVENT, BLOCK_EVENT).
 * @return int Returns SUCCESS (0) on successful switch, or FAILURE (-1) on error.
 */
int schedule_event(int event);


/**
 * @brief Selects the next thread from the ready queue and makes it RUNNING.
 *
 * Pops the front thread from the `ready_queue`, marks it as RUNNING,
 * and increases its quantum count by one.
 * This is usually called during a context switch after the current thread yields.
 *
 * @return int Returns SUCCESS if a thread is found, otherwise FAILURE.
 */
int set_next_thread_as_running();


/**
 * @brief Updates the sleep timers of all sleeping threads.
 *
 * Decreases the sleep counter for each thread in the `sleep_map`.
 * When a thread's sleep duration reaches zero, it is moved:
 *   - From SLEEP → READY
 *   - From BLOKED_AND_SLEEP → BLOCKED
 *
 * Threads ready to run are pushed back into the `ready_queue`.
 */
void advance_sleep_quantum();


#endif //PRATFOLIO_SCHEDULER_H
