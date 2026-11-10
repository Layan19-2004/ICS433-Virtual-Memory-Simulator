
#include <stdio.h>
#include <assert.h>
#include "replacement.h"

/*
 * ICS 433 - Virtual Memory Simulator
 *
 * File: test_replacement.c
 *
 * Purpose:
 * Automatically test FIFO and LRU page-replacement
 * algorithms under different memory conditions.
 */

/*
 * Check that the algorithm produces the expected
 * number of hits, faults, and replacements.
 */
void check_results(Statistics result,
                   int hits,
                   int faults,
                   int replacements) {

    assert(result.hits == hits);
    assert(result.faults == faults);
    assert(result.replacements == replacements);
}

int main(void) {

    // Test 1: Compare FIFO and LRU.
    int pages1[] = {1, 2, 3, 1, 4, 1};

    Statistics fifo1 = run_fifo(pages1, 6, 3);
    Statistics lru1 = run_lru(pages1, 6, 3);

    check_results(fifo1, 1, 5, 2);
    check_results(lru1, 2, 4, 1);

    printf("Test 1 passed: FIFO vs LRU\n");

    // Test 2: All requests are for the same page.
    int pages2[] = {2, 2, 2, 2};

    Statistics fifo2 = run_fifo(pages2, 4, 3);
    Statistics lru2 = run_lru(pages2, 4, 3);

    check_results(fifo2, 3, 1, 0);
    check_results(lru2, 3, 1, 0);

    printf("Test 2 passed: Repeated page\n");

    // Test 3: Only one physical frame.
    int pages3[] = {1, 2, 1};

    Statistics fifo3 = run_fifo(pages3, 3, 1);
    Statistics lru3 = run_lru(pages3, 3, 1);

    check_results(fifo3, 0, 3, 2);
    check_results(lru3, 0, 3, 2);

    printf("Test 3 passed: Single frame\n");

    // Test 4: More frames than distinct pages.
    int pages4[] = {1, 2, 3, 1, 2, 3};

    Statistics fifo4 = run_fifo(pages4, 6, 5);
    Statistics lru4 = run_lru(pages4, 6, 5);

    check_results(fifo4, 3, 3, 0);
    check_results(lru4, 3, 3, 0);

    printf("Test 4 passed: Sufficient frames\n");

    // Test 5: Invalid frame count.
    int pages5[] = {1, 2, 3};

    Statistics fifo5 = run_fifo(pages5, 3, 0);
    Statistics lru5 = run_lru(pages5, 3, 0);

    check_results(fifo5, 0, 0, 0);
    check_results(lru5, 0, 0, 0);

    printf("Test 5 passed: Invalid frame count\n");

    // All tests completed without assertion failures.
    printf("\nAll FIFO and LRU tests passed!\n");

    return 0;
}
