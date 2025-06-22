//
// Created by ommri on 6/22/2025.
//

#include "signals.h"

// signals functions //

int make_signals_set() {
    sigemptyset(&sa.sa_mask);
    sigemptyset(&sig_set);
    sigaddset(&sig_set, SIGVTALRM);
    return SUCCESS;
}

int change_signal_mask(int action) {
    if (sigprocmask(action, &sig_set, NULL) == FAILURE) {
        SYSTEM_ERROR("sigprocmask error", finish_program(ERROR));
        return FAILURE;
    }
    return SUCCESS;
}


// Timer functions //
void time_handler(int sig) {
    schedule_event(PREEMPT_EVENT);


}

int start_timer() {
    timer.it_value.tv_sec =
            (init_cuantom_usecs) / SECOND;        // first time interval, seconds part
    timer.it_value.tv_usec = (init_cuantom_usecs)
                             % SECOND;        // first time interval, microseconds part
    timer.it_interval.tv_sec =
            (init_cuantom_usecs) / SECOND;    // following time intervals, seconds part
    timer.it_interval.tv_usec = (init_cuantom_usecs)
                                % SECOND;    // following time intervals, microseconds part

    if (setitimer(ITIMER_VIRTUAL, &timer, NULL) < 0) {
        SYSTEM_ERROR("setitimer error", finish_program(ERROR));
    }
    return SUCCESS;
}

int initialize_timer() {

    sa.sa_handler = &time_handler;

    if (sigaction(SIGVTALRM, &sa, NULL) < 0) {
        SYSTEM_ERROR("sigaction error", finish_program(
                ERROR));
        return FAILURE;
    }
    if (start_timer() == FAILURE) {
        SYSTEM_ERROR("restart_timer error", finish_program(ERROR));

    }
    return SUCCESS;
}

