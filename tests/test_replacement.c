
#include <stdio.h>
#include "replacement.h"

/*
 * ICS 433 - Virtual Memory Simulator
 *
 * File: test_replacement.c
 *
 * Purpose:
 * Test FIFO and LRU using the same reference sequence.
 */

int main(void) {

    // Reference sequence used by both algorithms.
    int pages[] = {1, 2, 3, 1, 4, 1};

    // Number of requests and available frames.
    int page_count = 6;
    int frame_count = 3;

    // Run both algorithms using identical inputs.
    Statistics fifo = run_fifo(pages, page_count, frame_count);
    Statistics lru = run_lru(pages, page_count, frame_count);

    // Display FIFO results.
    printf("===== FIFO Results =====\n");
    printf("Page Hits: %d\n", fifo.hits);
    printf("Page Faults: %d\n", fifo.faults);
    printf("Replacements: %d\n\n", fifo.replacements);

    // Display LRU results.
    printf("===== LRU Results =====\n");
    printf("Page Hits: %d\n", lru.hits);
    printf("Page Faults: %d\n", lru.faults);
    printf("Replacements: %d\n", lru.replacements);

    return 0;
}
