
#include <stdio.h>
#include "replacement.h"

/*
 * ICS 433 - Virtual Memory Simulator
 *
 * File: test_replacement.c
 *
 * Purpose:
 * Test the FIFO page-replacement algorithm
 * and verify its results.
 */

int main(void) {

    // Page-reference sequence used for testing.
    int pages[] = {1, 2, 3, 1, 4};

    // Total number of page requests.
    int page_count = 5;

    // Number of available physical memory frames.
    int frame_count = 3;

    // Run the FIFO algorithm.
    Statistics result = run_fifo(pages, page_count, frame_count);

    // Display the simulation results.
    printf("===== FIFO Results =====\n");
    printf("Page Hits: %d\n", result.hits);
    printf("Page Faults: %d\n", result.faults);
    printf("Replacements: %d\n", result.replacements);

    return 0;
}
