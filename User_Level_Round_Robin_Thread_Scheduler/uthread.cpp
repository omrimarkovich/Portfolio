#include "uthread.h"





/**
 * @brief Initializes the thread library.
 *
 * @param quantum_usecs The length of a quantum in microseconds.
 * @return 0 on success, -1 on failure.
 */
int uthread_init(int quantum_usecs) {
    if (quantum_usecs <= 0) {
        FUNCTION_ERROR("quantum_usecs must be positive", FAILURE);
    }

    make_signals_set();

// Initialize the threads array and main thread
    Thread *main_thread = new Thread(0, RUNNING, 1, nullptr, nullptr);
    if (main_thread == nullptr) {
        SYSTEM_ERROR("malloc error", finish_program(ERROR));
    }

    threads_array[MAIN_THREAD_ID] = main_thread;

//initialize timer
    init_cuantom_usecs = quantum_usecs;
    initialize_timer();

    return SUCCESS;
}

/**
 * @brief Creates a new thread.
 *
 * @param entry_point The function the thread will execute.
 * @return The thread ID on success, -1 on failure.
 */
int uthread_spawn(thread_entry_point entry_point) {

    change_signal_mask(SIG_BLOCK);

    if (entry_point == nullptr) {
        FUNCTION_ERROR("entry_point cannot be null", FAILURE);
    }
    delete_all_deleted_threads_arr();
    int new_tid = the_smallest_place_available_thread();

    if (new_tid == FAILURE) {
        FUNCTION_ERROR("No available thread reached to maximum", FAILURE);
    }
//    new thread stack and thread creation
    char *stack = new char[STACK_SIZE]; // Allocate stack for the new thread
    Thread *new_thread = new Thread(new_tid, READY, 0, stack, entry_point);
    if (new_thread == nullptr) {
        SYSTEM_ERROR("Malloc error", finish_program(ERROR));
    }

    threads_array[new_tid] = new_thread;
    ready_queue.push(new_thread->getTid()); // Add the new thread to the ready queue



// unblock signal sig_set
    change_signal_mask(SIG_UNBLOCK);
    return new_thread->getTid();
}

/**
 * @brief Terminates the thread with the given ID.
 *
 * @param tid The thread ID to terminate.
 * @return 0 on success, -1 on failure.
 */

int uthread_terminate(int tid) {

    change_signal_mask(SIG_BLOCK);
    if (validate_thread_exists(tid) == FAILURE) {
        return FAILURE;
    }
    int status = threads_array[tid]->getStatus();

    deleted_threads.insert(tid);
    if (tid == MAIN_THREAD_ID) {
        finish_program(
                CLEAR); // Terminate the entire process
        return SUCCESS;
    }
    if (status == READY){
        erase_tid_from_ready_queue(tid);
    }
    if (status == SLEEP || status == BLOKED_AND_SLEEP){
        sleep_map.erase(tid);
    }

    if (status == RUNNING) {
        schedule_event(TERMINATE_EVENT);
    }




    return change_signal_mask(SIG_UNBLOCK);
}

/**
 * @brief Blocks the thread with the given ID.
 *
 * @param tid The thread ID to block.
 * @return 0 on success, -1 on failure.
 */
int uthread_block(int tid) {
    change_signal_mask(SIG_BLOCK);
    if (validate_thread_exists(tid) == FAILURE) {
        return FAILURE;
    }
    if (tid == MAIN_THREAD_ID) {
        FUNCTION_ERROR("Cannot block the main thread", FAILURE);
    }

    if ((int)current_thread_id == tid) {
        schedule_event(BLOCK_EVENT);
    }
    switch (threads_array[tid]->getStatus()) {
        case READY:
            erase_tid_from_ready_queue(tid);
            threads_array[tid]->setStatus( BLOCKED);
            break;
        case SLEEP:
            threads_array[tid]->setStatus( BLOKED_AND_SLEEP);
            break;
        case BLOKED_AND_SLEEP:
            break;
        case BLOCKED:
            break;
        case RUNNING:
            break;
    }
    return change_signal_mask(SIG_UNBLOCK);
}

/**
 * @brief Resumes the thread with the given ID.
 *
 * @param tid The thread ID to resume.
 * @return 0 on success, -1 on failure.
 */
int uthread_resume(int tid) {
    change_signal_mask(SIG_BLOCK);

    if (validate_thread_exists(tid) == FAILURE) {
        return FAILURE;
    }

    switch (threads_array[tid]->getStatus()) {
        case BLOCKED:
            threads_array[tid]->setStatus( READY);
            ready_queue.push(tid); // Add the thread to the ready queue
            break;
        case BLOKED_AND_SLEEP:
            threads_array[tid]->setStatus( SLEEP);
            break;
        default: FUNCTION_ERROR("Thread is not in a blockable state", FAILURE);

    }
    return change_signal_mask(SIG_UNBLOCK);
}

/**
 * @brief Puts the thread to sleep for a specified number of quantums.
 *
 * @param num_quantums The number of quantums to sleep.
 * @return 0 on success, -1 on failure.
 */
int uthread_sleep(int num_quantums) {
    change_signal_mask(SIG_BLOCK);
    if (current_thread_id == MAIN_THREAD_ID) {
        FUNCTION_ERROR("Cannot block the main thread", FAILURE);
    }
    if (num_quantums < 0) {
        FUNCTION_ERROR("num_quantums must be positive", FAILURE);
    }
    if (num_quantums == 0) {
        //add to ready queue
        threads_array[current_thread_id]->setStatus(READY);
        ready_queue.push(current_thread_id); // Add the thread to the ready queue
        return change_signal_mask(SIG_UNBLOCK);
    }
    //assert cur thread is in running state
    sleep_map[current_thread_id] = num_quantums + 1;
    schedule_event(SLEEP_EVENT);

    return change_signal_mask(SIG_UNBLOCK);
}

/**
 * @brief Returns the ID of the currently running thread.
 *
 * @return The thread ID of the currently running thread.
 */
int uthread_get_tid() {
    return current_thread_id;
}

/**
 * @brief Returns the total number of quantums since the library was initialized.
 *
 * @return The total number of quantums.
 */
int uthread_get_total_quantums() {
    return total_quantums;
}

/**
 * @brief Returns the number of quantums the thread with the given ID has run.
 *
 * @param tid The thread ID.
 * @return The number of quantums the thread has run, or -1 on failure.
 */
int uthread_get_quantums(int tid) {
    if (validate_thread_exists(tid) == FAILURE) {
        return FAILURE;
    }
    return threads_array[tid]->getQuantumCount();
}


