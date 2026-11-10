#include "metrics.h"

#include <stdio.h>

void metrics_init(Metrics *metrics) {
    if (metrics != NULL) {
        *metrics = (Metrics){0, 0, 0, 0, 0};
    }
}

Metrics metrics_from_statistics(Statistics statistics,
                                int references,
                                int write_backs) {
    Metrics metrics;

    metrics.references = references > 0 ? references : 0;
    metrics.hits = statistics.hits >= 0 ? statistics.hits : 0;
    metrics.faults = statistics.faults >= 0 ? statistics.faults : 0;
    metrics.replacements =
        statistics.replacements >= 0 ? statistics.replacements : 0;
    metrics.write_backs = write_backs >= 0 ? write_backs : 0;

    return metrics;
}

void metrics_record_hit(Metrics *metrics) {
    if (metrics != NULL) {
        metrics->references++;
        metrics->hits++;
    }
}

void metrics_record_fault(Metrics *metrics, int was_replacement) {
    if (metrics != NULL) {
        metrics->references++;
        metrics->faults++;

        if (was_replacement) {
            metrics->replacements++;
        }
    }
}

void metrics_record_write_back(Metrics *metrics) {
    if (metrics != NULL) {
        metrics->write_backs++;
    }
}

double metrics_hit_rate(const Metrics *metrics) {
    if (metrics == NULL || metrics->references <= 0) {
        return 0.0;
    }

    return (double)metrics->hits / metrics->references;
}

double metrics_fault_rate(const Metrics *metrics) {
    if (metrics == NULL || metrics->references <= 0) {
        return 0.0;
    }

    return (double)metrics->faults / metrics->references;
}

void metrics_print(const char *algorithm, const Metrics *metrics) {
    if (metrics == NULL) {
        return;
    }

    printf("\n%s Performance Summary\n",
           algorithm != NULL ? algorithm : "Algorithm");

    printf("References:   %d\n", metrics->references);
    printf("Hits:         %d\n", metrics->hits);
    printf("Faults:       %d\n", metrics->faults);
    printf("Replacements: %d\n", metrics->replacements);
    printf("Write-backs:  %d\n", metrics->write_backs);

    printf("Hit rate:     %.2f%%\n",
           metrics_hit_rate(metrics) * 100.0);

    printf("Fault rate:   %.2f%%\n",
           metrics_fault_rate(metrics) * 100.0);
}

void metrics_print_comparison(const Metrics *results,
                              int result_count) {
    if (results == NULL || result_count <= 0) {
        return;
    }

    printf("\nAlgorithm Comparison\n");

    printf("%-12s %-12s %-12s %-15s %-12s\n",
           "Algorithm",
           "Hits",
           "Faults",
           "Replacements",
           "Fault Rate");

    for (int i = 0; i < result_count; i++) {
        printf("%-12d %-12d %-12d %-15d %.2f%%\n",
               i + 1,
               results[i].hits,
               results[i].faults,
               results[i].replacements,
               metrics_fault_rate(&results[i]) * 100.0);
    }
}