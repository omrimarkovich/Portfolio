//
// Created by ommri on 6/22/2025.
//

#include "utils.h"


int validate_thread_exists(int tid) {
    if (tid < 0 || tid >= MAX_THREAD_NUM) {
        FUNCTION_ERROR("Invalid thread ID", FAILURE);
    }
    if (threads_array[tid] == nullptr) {
        FUNCTION_ERROR("Thread does not exist", FAILURE);
    }
    return SUCCESS;
}


void delete_thread_from_memory(int tid) {
    if (threads_array[tid] != nullptr) {
        delete threads_array[tid];
        threads_array[tid] = nullptr;
    }
}

void delete_all_deleted_threads_arr()
{
    for (auto it = deleted_threads.begin(); it != deleted_threads.end(); )
    {
        delete_thread_from_memory(*it);
        it = deleted_threads.erase(it);
    }
}

int the_smallest_place_available_thread() {
    for (int i = 0; i < MAX_THREAD_NUM; i++) {
        if (threads_array[i] == nullptr) {
            return i;
        }
    }
    return FAILURE;
}

void erase_tid_from_ready_queue(unsigned int tid) {
    std::queue<unsigned int> temp_queue;
    while (!ready_queue.empty()) {
        unsigned int current_tid = ready_queue.front();
        ready_queue.pop();
        if (current_tid != tid) {
            temp_queue.push(current_tid);
        }
    }
    ready_queue = temp_queue; // Update the original queue
}

void finish_program(int status) {
    for (int i = MAX_THREAD_NUM-1; i >= 0; i--)
    {

        if (threads_array[i] != nullptr) {

            if ((unsigned int)i == current_thread_id) {
                if (current_thread_id != 0) {
                    schedule_event(TERMINATE_EVENT);
                }
                break;
            }

            delete_thread_from_memory(i);
        }
    }
    std::exit(status);
}
