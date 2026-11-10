
#include "../include/replacement.h"
#include <stdlib.h>

/*
 * ICS 433 - Virtual Memory Simulator
 *
 * File: fifo.c
 *
 * Purpose:
 * Implements the FIFO page-replacement algorithm.
 * FIFO removes the page that entered memory first
 * whenever all physical frames are occupied.
 */

Statistics run_fifo(const int pages[], int page_count, int frame_count) {

    // Initialize all statistics to zero.
    Statistics stats = {0, 0, 0};

    // Check that the input values are valid.
    if (pages == NULL || page_count <= 0 || frame_count <= 0) {
        return stats;
    }

    // Allocate memory for the physical frames.
    int *frames = malloc((size_t)frame_count * sizeof(int));

    // Stop if memory allocation fails.
    if (frames == NULL) {
        return stats;
    }

    // Number of frames currently containing pages.
    int occupied = 0;

    // Index of the next frame to replace when memory is full.
    int next_replace = 0;

    // Process each page request in the reference sequence.
    for (int i = 0; i < page_count; i++) {

        int current_page = pages[i];

        // Assume the requested page is not in memory.
        int found = 0;

        // Search the occupied frames for the requested page.
        for (int j = 0; j < occupied; j++) {
            if (frames[j] == current_page) {
                found = 1;
                break;
            }
        }

        if (found) {
            // Page hit: the requested page is already in RAM.
            stats.hits++;
        }
        else {
            // Page fault: the requested page is not in RAM.
            stats.faults++;

            if (occupied < frame_count) {

                // An empty frame is available.
                // Load the requested page without replacement.
                frames[occupied] = current_page;
                occupied++;
            }
            else {

                // All frames are occupied.
                // Replace the page that entered memory first.
                frames[next_replace] = current_page;

                // Move to the next frame in circular order.
                next_replace = (next_replace + 1) % frame_count;

                // Count the page replacement.
                stats.replacements++;
            }
        }
    }

    // Release dynamically allocated memory.
    free(frames);

    // Return the final hit, fault, and replacement counts.
    return stats;
}
