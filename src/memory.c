#include "memory.h"
#include <stdlib.h>
#include <stdint.h>

/* Check that the frame index is valid. */
static int valid_index(const Memory *memory, int index) {
    return memory != NULL &&
           memory->frames != NULL &&
           index >= 0 &&
           index < memory->frame_count;
}

/* Allocate and initialize physical memory. */
int memory_init(Memory *memory, int frame_count) {
    if (memory == NULL) {
        return -1;
    }

    *memory = (Memory){0};

    if (frame_count <= 0 ||
        (size_t)frame_count > SIZE_MAX / sizeof(Frame)) {
        return -1;
    }

    memory->frames = calloc((size_t)frame_count, sizeof(Frame));

    if (memory->frames == NULL) {
        return -1;
    }

    memory->frame_count = frame_count;

    return 0;
}

/* Release allocated memory. */
void memory_destroy(Memory *memory) {
    if (memory == NULL) {
        return;
    }

    free(memory->frames);

    *memory = (Memory){0};
}

/* Find a page belonging to a specific process. */
int memory_find_page(const Memory *memory,
                     int process_id,
                     int page_number) {
    if (memory == NULL || memory->frames == NULL) {
        return -1;
    }

    for (int i = 0; i < memory->frame_count; i++) {
        if (memory->frames[i].occupied &&
            memory->frames[i].process_id == process_id &&
            memory->frames[i].page_number == page_number) {
            return i;
        }
    }

    return -1;
}

/* Find an unused frame. */
int memory_find_free_frame(const Memory *memory) {
    if (memory == NULL || memory->frames == NULL) {
        return -1;
    }

    for (int i = 0; i < memory->frame_count; i++) {
        if (!memory->frames[i].occupied) {
            return i;
        }
    }

    return -1;
}

/* Update a page that is already in memory. */
int memory_touch(Memory *memory, int index, int is_write) {
    if (!valid_index(memory, index) ||
        !memory->frames[index].occupied ||
        (is_write != 0 && is_write != 1)) {
        return -1;
    }

    memory->frames[index].reference_bit = 1;

    if (is_write) {
        memory->frames[index].dirty = 1;
    }

    return 0;
}

/* Load a missing page into the selected frame. */
int memory_load_page(Memory *memory,
                     int index,
                     int process_id,
                     int page_number,
                     int is_write) {
    if (!valid_index(memory, index) ||
        (is_write != 0 && is_write != 1)) {
        return -1;
    }

    Frame *frame = &memory->frames[index];

    int write_back = frame->occupied && frame->dirty;

    if (write_back) {
        memory->write_backs++;
    }

    if (!frame->occupied) {
        memory->used_frames++;
    }

    frame->process_id = process_id;
    frame->page_number = page_number;
    frame->occupied = 1;
    frame->dirty = is_write;
    frame->reference_bit = 1;

    return write_back;
}
