
#ifndef REPLACEMENT_H
#define REPLACEMENT_H

/*
 * ICS 433 - Virtual Memory Simulator
 *
 * File: replacement.h
 *
 * Purpose:
 * This header file defines the shared data structures
 * and function declarations for the FIFO and LRU
 * page-replacement algorithms.
 */

/*
 * Statistics structure
 *
 * Stores the results of a page-replacement simulation.
 *
 * hits         - Number of pages already found in memory.
 * faults       - Number of requested pages not in memory.
 * replacements - Number of pages removed when memory is full.
 */
typedef struct {
    int hits;
    int faults;
    int replacements;
} Statistics;

/*
 * FIFO (First-In, First-Out)
 *
 * Replaces the page that has been in memory the longest.
 *
 * Parameters:
 * pages[]     - Array containing the page-reference sequence.
 * page_count  - Total number of page requests.
 * frame_count - Number of available physical memory frames.
 *
 * Returns:
 * A Statistics structure containing the simulation results.
 */
Statistics run_fifo(const int pages[],
                    int page_count,
                    int frame_count);

/*
 * LRU (Least Recently Used)
 *
 * Replaces the page that has not been accessed
 * for the longest period of time.
 *
 * Parameters:
 * pages[]     - Array containing the page-reference sequence.
 * page_count  - Total number of page requests.
 * frame_count - Number of available physical memory frames.
 *
 * Returns:
 * A Statistics structure containing the simulation results.
 */
Statistics run_lru(const int pages[],
                   int page_count,
                   int frame_count);

#endif
