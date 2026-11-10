#include "memory.h"
#include "replacement.h"
#include <stddef.h>

/* Process one page reference using Clock. */
int clock_access(Memory *memory,
                 int process_id,
                 int page_number,
                 int is_write,
                 MemoryAccess *result) {

    if (result == NULL) {
        return -1;
    }

    /* Initialize the result of this access. */
    *result = (MemoryAccess){
        .hit = 0,
        .fault = 0,
        .replacement = 0,
        .write_back = 0,
        .frame_index = -1,
        .evicted_process_id = -1,
        .evicted_page = -1
    };

    /* Validate memory and the operation type. */
    if (memory == NULL ||
        memory->frames == NULL ||
        memory->frame_count <= 0 ||
        memory->clock_hand < 0 ||
        memory->clock_hand >= memory->frame_count ||
        (is_write != 0 && is_write != 1)) {
        return -1;
    }

    /* Search for the requested page. */
    int index = memory_find_page(memory,
                                 process_id,
                                 page_number);

    if (index >= 0) {
        /* Page hit: update reference and dirty bits. */
        result->hit = 1;
        result->frame_index = index;

        return memory_touch(memory, index, is_write);
    }

    /* Page fault: the page must be loaded. */
    result->fault = 1;

    index = memory_find_free_frame(memory);

    if (index < 0) {
        /* Memory is full: give referenced pages a second chance. */
        while (memory->frames[memory->clock_hand].reference_bit) {
            memory->frames[memory->clock_hand].reference_bit = 0;

            memory->clock_hand =
                (memory->clock_hand + 1) % memory->frame_count;
        }

        /* The hand now points to a page with reference bit 0. */
        index = memory->clock_hand;

        result->replacement = 1;
        result->evicted_process_id =
            memory->frames[index].process_id;
        result->evicted_page =
            memory->frames[index].page_number;
    }

    /* Load the new page and record any dirty-page write-back. */
    result->frame_index = index;

    result->write_back = memory_load_page(memory,
                                          index,
                                          process_id,
                                          page_number,
                                          is_write);

    /* Move the hand to the next frame in circular order. */
    memory->clock_hand = (index + 1) % memory->frame_count;

    return 0;
}

/*
 * Matches the existing FIFO and LRU interface.
 * This wrapper treats all references as reads by process 0.
 * Use clock_access directly for multiple processes and writes.
 */
Statistics run_clock(const int pages[],
                     int page_count,
                     int frame_count) {

    Statistics stats = {0, 0, 0};

    if (pages == NULL || page_count <= 0 || frame_count <= 0) {
        return stats;
    }

    Memory memory;

    if (memory_init(&memory, frame_count) != 0) {
        return stats;
    }

    for (int i = 0; i < page_count; i++) {
        MemoryAccess result;

        clock_access(&memory, 0, pages[i], 0, &result);

        stats.hits += result.hit;
        stats.faults += result.fault;
        stats.replacements += result.replacement;
    }

    memory_destroy(&memory);

    return stats;
}
