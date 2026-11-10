#include "thrashing.h"

#include <stdio.h>
#include <stdlib.h>

static int add_if_new(int pages[],
                      int *count,
                      int page) {
    for (int i = 0; i < *count; i++) {
        if (pages[i] == page) {
            return 0;
        }
    }

    pages[*count] = page;
    (*count)++;

    return 1;
}

int estimate_working_set(const int pages[],
                         int page_count,
                         int window_size) {
    if (pages == NULL ||
        page_count <= 0 ||
        window_size <= 0) {
        return 0;
    }

    int start = 0;

    if (page_count > window_size) {
        start = page_count - window_size;
    }

    int window_length = page_count - start;

    int *unique_pages =
        malloc((size_t)window_length * sizeof(int));

    if (unique_pages == NULL) {
        return 0;
    }

    int unique_count = 0;

    for (int i = start; i < page_count; i++) {
        add_if_new(unique_pages,
                   &unique_count,
                   pages[i]);
    }

    free(unique_pages);

    return unique_count;
}

int estimate_process_working_set(
    const PageReference references[],
    int reference_count,
    int process_id,
    int window_size
) {
    if (references == NULL ||
        reference_count <= 0 ||
        window_size <= 0) {
        return 0;
    }

    int *pages =
        malloc((size_t)reference_count * sizeof(int));

    if (pages == NULL) {
        return 0;
    }

    int page_count = 0;

    for (int i = 0; i < reference_count; i++) {
        if (references[i].process_id == process_id) {
            pages[page_count] =
                references[i].page_number;

            page_count++;
        }
    }

    int result = estimate_working_set(pages,
                                      page_count,
                                      window_size);

    free(pages);

    return result;
}

int detect_thrashing(double fault_rate,
                     int working_set_size,
                     int allocated_frames,
                     double fault_threshold) {
    if (fault_threshold < 0.0) {
        fault_threshold =
            DEFAULT_THRASHING_FAULT_THRESHOLD;
    }

    return fault_rate >= fault_threshold &&
           working_set_size > allocated_frames;
}

ThrashingResult analyze_thrashing(
    int process_id,
    const PageReference references[],
    int reference_count,
    int window_size,
    int allocated_frames,
    const Metrics *metrics,
    double fault_threshold
) {
    ThrashingResult result = {
        process_id,
        window_size,
        0,
        allocated_frames,
        0.0,
        0
    };

    result.working_set_size =
        estimate_process_working_set(
            references,
            reference_count,
            process_id,
            window_size
        );

    if (metrics != NULL) {
        result.fault_rate =
            metrics_fault_rate(metrics);
    }

    result.possible_thrashing =
        detect_thrashing(
            result.fault_rate,
            result.working_set_size,
            result.allocated_frames,
            fault_threshold
        );

    return result;
}

void print_thrashing_result(
    const ThrashingResult *result
) {
    if (result == NULL) {
        return;
    }

    printf("\nProcess %d Working-Set Analysis\n",
           result->process_id);

    printf("Window size:       %d\n",
           result->window_size);

    printf("Working-set size:   %d pages\n",
           result->working_set_size);

    printf("Allocated frames:   %d\n",
           result->allocated_frames);

    printf("Fault rate:         %.2f%%\n",
           result->fault_rate * 100.0);

    printf("Possible thrashing: %s\n",
           result->possible_thrashing ? "YES" : "NO");
}