
#include "replacement.h"
#include <stdlib.h>

/*
 * ICS 433 - Virtual Memory Simulator
 *
 * File: lru.c
 *
 * Purpose:
 * Implements the Least Recently Used (LRU)
 * page-replacement algorithm.
 *
 * LRU replaces the page that has not been
 * accessed for the longest time.
 */

Statistics run_lru(const int pages[], int page_count, int frame_count) {

    // Initialize all statistics to zero.
    Statistics stats = {0, 0, 0};

    // Validate the input parameters.
    if (pages == NULL || page_count <= 0 || frame_count <= 0) {
        return stats;
    }

    // Allocate memory for the physical frames.
    int *frames = malloc((size_t)frame_count * sizeof(int));

    // Store the last access time for each frame.
    int *last_used = malloc((size_t)frame_count * sizeof(int));

    // Check whether memory allocation was successful.
    if (frames == NULL || last_used == NULL) {
        free(frames);
        free(last_used);
        return stats;
    }

    // Number of frames currently containing pages.
    int occupied = 0;

    // Process every page in the reference sequence.
    for (int i = 0; i < page_count; i++) {

        int current_page = pages[i];

        // Assume the requested page is not in RAM.
        int found_index = -1;

        // Search the occupied frames for the requested page.
        for (int j = 0; j < occupied; j++) {
            if (frames[j] == current_page) {
                found_index = j;
                break;
            }
        }

        if (found_index != -1) {

            // Page hit: the requested page is already in RAM.
            stats.hits++;

            // Update its last access time.
            last_used[found_index] = i;
        }
        else {

            // Page fault: the requested page is not in RAM.
            stats.faults++;

            if (occupied < frame_count) {

                // Load the page into an available frame.
                frames[occupied] = current_page;

                // Record when the page was accessed.
                last_used[occupied] = i;

                occupied++;
            }
            else {

                // All frames are occupied.
                // Start by assuming frame 0 is least recently used.
                int lru_index = 0;

                // Find the frame with the oldest access time.
                for (int j = 1; j < frame_count; j++) {
                    if (last_used[j] < last_used[lru_index]) {
                        lru_index = j;
                    }
                }

                // Replace the least recently used page.
                frames[lru_index] = current_page;

                // Record the new page's access time.
                last_used[lru_index] = i;

                // Count the replacement.
                stats.replacements++;
            }
        }
    }

    // Release allocated memory.
    free(frames);
    free(last_used);

    // Return the simulation statistics.
    return stats;
}
