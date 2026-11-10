#include "cli.h"
#include "trace.h"

#include <stdio.h>
#include <string.h>

int main(void) {
    Algorithm algorithm;
    int frame_count;
    int input_mode;
    Trace trace = {NULL, 0};
    char filename[256];

    printf("Virtual Memory Simulator\n");

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

    printf("\nSelected algorithm: ");

    switch (algorithm) {
        case ALGORITHM_FIFO:
            printf("FIFO\n");
            break;

        case ALGORITHM_LRU:
            printf("LRU\n");
            break;

        case ALGORITHM_CLOCK:
            printf("Clock\n");
            break;
    }

    printf("Number of frames: %d\n\n", frame_count);
    print_trace(&trace);

    free_trace(&trace);

    return 0;
}