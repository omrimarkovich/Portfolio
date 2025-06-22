//
// Created by ommri on 6/22/2025.
//

#include "scheduler.h"


int schedule_event(int event) {

    change_signal_mask(SIG_BLOCK);
    int ret_val;


    switch (event) {
        case PREEMPT_EVENT:
            if (ready_queue.empty()) {
                delete_all_deleted_threads_arr();
                threads_array[current_thread_id]->setQuantumCount();
                total_quantums++;
                advance_sleep_quantum();
                return change_signal_mask(SIG_UNBLOCK);
            }
            ret_val = sigsetjmp(threads_array[current_thread_id]->getEnv(), 1);
            if (ret_val == 0) {
                // Save the current thread's context -
                threads_array[current_thread_id]->setStatus(READY);
                ready_queue.push(current_thread_id); // Add the current thread to the ready queue

                // Switch to the next thread in the ready queue
                if (set_next_thread_as_running() == FAILURE) {
                    FUNCTION_ERROR("no threads in the ready queue", FAILURE)
                }
                // Restore the next thread's context
            }
            if (ret_val == 1) {
                delete_all_deleted_threads_arr();
                return change_signal_mask(SIG_UNBLOCK); // Unblock the signal
            }
            break;

        case BLOCK_EVENT:
            // Save the current thread's context
            ret_val = sigsetjmp(threads_array[current_thread_id]->getEnv(), 1);
            if (ret_val == 0) {
                threads_array[current_thread_id]->setStatus(BLOCKED);
                //call advance ready queue
                if (set_next_thread_as_running() == FAILURE) {
                    FUNCTION_ERROR("no threads in the ready queue", FAILURE)
                }
            }
            if (ret_val == 1) {
                delete_all_deleted_threads_arr();
                return change_signal_mask(SIG_UNBLOCK); //
            }
            break;

        case SLEEP_EVENT:
            ret_val = sigsetjmp(threads_array[current_thread_id]->getEnv(), 1);
            if (ret_val == 0) {
                threads_array[current_thread_id]->setStatus(SLEEP);
                //call advance ready queue
                if (set_next_thread_as_running() == FAILURE) {
                    FUNCTION_ERROR("no threads in the ready queue", FAILURE)
                }
            }
            if (ret_val == 1) {
                delete_all_deleted_threads_arr();
                return change_signal_mask(SIG_UNBLOCK);
            }
            break;

        case TERMINATE_EVENT:
            //call advance ready queue
            if (set_next_thread_as_running() == FAILURE) {
                FUNCTION_ERROR("no threads in the ready queue", FAILURE)
            }
            break;

    }

    total_quantums++;
    advance_sleep_quantum();
    initialize_timer();
    change_signal_mask(SIG_UNBLOCK);
    siglongjmp(threads_array[current_thread_id]->getEnv(), 1);


    return SUCCESS;
}

int set_next_thread_as_running() {
    if (ready_queue.empty()) {
        return FAILURE;
    }
    current_thread_id = ready_queue.front();
    ready_queue.pop();
    threads_array[current_thread_id]->setStatus(RUNNING) ;
    threads_array[current_thread_id]->setQuantumCount();
    return SUCCESS;
}




void advance_sleep_quantum() {
    if (sleep_map.empty()) {
        return;
    }

    for (auto it = sleep_map.begin(); it != sleep_map.end();) {
        if (it->second > 0) {
            it->second--;

            if (it->second == 0) {
                int tid = it->first;
                ThreadStatus &status = threads_array[tid]->getStatus();

                switch (status) {
                    case SLEEP:
                        status = READY;
                        ready_queue.push(tid); // Add to ready queue
                        it = sleep_map.erase(it); // Erase and advance iterator
                        continue;

                    case BLOKED_AND_SLEEP:
                        status = BLOCKED;
                        it = sleep_map.erase(it); // Erase and advance iterator
                        continue;

                    default:
                        // Optional: handle unexpected state
                        break;
                }
            }
        }
        ++it; // Advance only if not erased
    }
}


// Helper functions implementations //

