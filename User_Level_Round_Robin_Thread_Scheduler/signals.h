//
// Created by ommri on 6/22/2025.
//

#ifndef PRATFOLIO_SIGNALS_H
#define PRATFOLIO_SIGNALS_H

#include "Globals.h"
#include "utils.h"
#include "scheduler.h"

#include <signal.h>
#include <sys/time.h>
#include <setjmp.h>
#include <csignal>   // Optional: C++ version of <signal.h>

/**
 * @brief Initializes the signal sets used by the library.
 *
 * This function sets up `sig_set` with the appropriate signals (e.g., SIGVTALRM)
 * that will be blocked/unblocked during critical sections.
 *
 * @return SUCCESS (0) on success.
 */
int make_signals_set();

/**
 * @brief Initializes the virtual timer and installs the SIGVTALRM handler.
 *
 * This sets up a timer that sends SIGVTALRM at regular intervals to enable preemptive scheduling.
 *
 * @return SUCCESS (0) on success, FAILURE (-1) on error.
 */
int initialize_timer();

/**
 * @brief Changes the process signal mask for SIGVTALRM.
 *
 * This is used to block or unblock SIGVTALRM in critical sections using `SIG_BLOCK` or `SIG_UNBLOCK`.
 *
 * @param action Either SIG_BLOCK or SIG_UNBLOCK.
 * @return SUCCESS (0) on success, FAILURE (-1) on error.
 */
int change_signal_mask(int action);

/**
 * @brief Signal handler for SIGVTALRM.
 *
 * This function is triggered when the virtual timer expires.
 * It schedules the next thread via a PREEMPT_EVENT.
 *
 * @param sig Signal number (should be SIGVTALRM).
 */
void time_handler(int sig);

/**
 * @brief Starts or restarts the virtual timer for quantum intervals.
 *
 * Configures the `timer` struct and invokes `setitimer` with ITIMER_VIRTUAL.
 *
 * @return SUCCESS (0) on success, FAILURE (-1) on error.
 */
int start_timer();

#endif //PRATFOLIO_SIGNALS_H
