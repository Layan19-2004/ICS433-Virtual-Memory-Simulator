#include "cli.h"
#include "trace.h"
#include "replacement.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    Algorithm algorithm;
    int frame_count;
    int input_mode;
    Trace trace = {NULL, 0};
    char filename[256];
    int *pages;
    Statistics statistics;

    printf("Virtual Memory and Page-Replacement Simulator\n");

    algorithm = choose_algorithm();
    frame_count = read_frame_count();
    input_mode = choose_input_mode();

    if (input_mode == 1) {
        if (!enter_trace_manually(&trace)) {
            printf("Failed to enter trace.\n");
            return 1;
        }
    } else {
        printf("Enter trace file name: ");

        if (fgets(filename, sizeof(filename), stdin) == NULL) {
            return 1;
        }

        filename[strcspn(filename, "\n")] = '\0';

        if (!load_trace_from_file(filename, &trace)) {
            printf("Failed to load trace file.\n");
            return 1;
        }
    }

    if (trace.count == 0) {
        printf("The trace is empty.\n");
        free_trace(&trace);
        return 1;
    }

    pages = malloc(trace.count * sizeof(int));

    if (pages == NULL) {
        printf("Memory allocation failed.\n");
        free_trace(&trace);
        return 1;
    }

    for (int i = 0; i < trace.count; i++) {
        pages[i] = trace.references[i].page_number;
    }

    switch (algorithm) {
        case ALGORITHM_FIFO:
            statistics = run_fifo(
                pages,
                trace.count,
                frame_count
            );
            break;

        case ALGORITHM_LRU:
            statistics = run_lru(
                pages,
                trace.count,
                frame_count
            );
            break;

        case ALGORITHM_CLOCK:
            statistics = run_clock(
                pages,
                trace.count,
                frame_count
            );
            break;

        default:
            printf("Invalid algorithm.\n");
            free(pages);
            free_trace(&trace);
            return 1;
    }

    printf("\nSimulation Results\n");
    printf("------------------\n");
    printf("Page hits: %d\n", statistics.hits);
    printf("Page faults: %d\n", statistics.faults);
    printf("Replacements: %d\n", statistics.replacements);

    free(pages);
    free_trace(&trace);

    return 0;
}