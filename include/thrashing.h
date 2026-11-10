#ifndef THRASHING_H
#define THRASHING_H

#include "metrics.h"
#include "trace.h"

/*
 * A process may be considered thrashing when:
 * fault rate >= 50%, and
 * working-set size > allocated frames.
 */
#define DEFAULT_THRASHING_FAULT_THRESHOLD 0.50

typedef struct {
    int process_id;
    int window_size;
    int working_set_size;
    int allocated_frames;
    double fault_rate;
    int possible_thrashing;
} ThrashingResult;

int estimate_working_set(const int pages[],
                         int page_count,
                         int window_size);

int estimate_process_working_set(const PageReference references[],
                                 int reference_count,
                                 int process_id,
                                 int window_size);

int detect_thrashing(double fault_rate,
                     int working_set_size,
                     int allocated_frames,
                     double fault_threshold);

ThrashingResult analyze_thrashing(
    int process_id,
    const PageReference references[],
    int reference_count,
    int window_size,
    int allocated_frames,
    const Metrics *metrics,
    double fault_threshold
);

void print_thrashing_result(const ThrashingResult *result);

#endif