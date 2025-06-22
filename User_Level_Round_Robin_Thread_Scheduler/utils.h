

#ifndef PRATFOLIO_UTILS_H
#define PRATFOLIO_UTILS_H
#include "thread.h"
#include "scheduler.h"

#include <unordered_set>
#include <cstdlib>   // exit
#include <cstdio>    // fprintf

/**
 * @brief Validates that a thread with the given ID exists.
 *
 * Checks that the thread ID is within valid bounds and that the corresponding
 * thread exists in the global threads array.
 *
 * @param tid The thread ID to validate.
 * @return SUCCESS (0) if valid, FAILURE (-1) otherwise.
 */
int validate_thread_exists(int tid);

/**
 * @brief Deletes a thread from memory.
 *
 * Frees the memory allocated for the thread with the given ID and removes it from
 * the global thread array.
 *
 * @param tid The ID of the thread to delete.
 */
void delete_thread_from_memory(int tid);

/**
 * @brief Deletes all threads that are marked for deletion.
 *
 * Iterates over the global `deleted_threads` set and deletes each thread from memory.
 * This helps avoid memory leaks when threads are logically removed.
 */
void delete_all_deleted_threads_arr();

/**
 * @brief Finds the smallest available thread ID.
 *
 * Iterates through the thread array to find the smallest unused thread ID.
 *
 * @return The available thread ID, or FAILURE (-1) if the maximum number is reached.
 */
int the_smallest_place_available_thread();

/**
 * @brief Removes a thread from the ready queue.
 *
 * Rebuilds the ready queue without the thread with the given ID.
 *
 * @param tid The ID of the thread to remove.
 */
void erase_tid_from_ready_queue(unsigned int tid);

/**
 * @brief Terminates the entire program cleanly.
 *
 * Frees all memory and exits the program with the provided status code.
 *
 * @param status The exit status (e.g., CLEAR or ERROR).
 */
void finish_program(int status);


#endif //PRATFOLIO_UTILS_H
