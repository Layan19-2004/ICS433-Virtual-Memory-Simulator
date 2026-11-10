#ifndef METRICS_H
#define METRICS_H

#include "replacement.h"

typedef struct {
    int references;
    int hits;
    int faults;
    int replacements;
    int write_backs;
} Metrics;

void metrics_init(Metrics *metrics);

Metrics metrics_from_statistics(Statistics statistics,
                                int references,
                                int write_backs);

void metrics_record_hit(Metrics *metrics);
void metrics_record_fault(Metrics *metrics, int was_replacement);
void metrics_record_write_back(Metrics *metrics);

double metrics_hit_rate(const Metrics *metrics);
double metrics_fault_rate(const Metrics *metrics);

void metrics_print(const char *algorithm, const Metrics *metrics);

void metrics_print_comparison(const Metrics *results,
                              int result_count);

#endif