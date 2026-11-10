#ifndef MEMORY_H
#define MEMORY_H

/* Represents one physical memory frame. */
typedef struct {
    int process_id;
    int page_number;
    int occupied;
    int dirty;
    int reference_bit;
} Frame;

/* Represents the entire physical memory. */
typedef struct {
    Frame *frames;
    int frame_count;
    int used_frames;
    int clock_hand;
    unsigned long write_backs;
} Memory;

/* Records the result of one page access. */
typedef struct {
    int hit;
    int fault;
    int replacement;
    int write_back;
    int frame_index;
    int evicted_process_id;
    int evicted_page;
} MemoryAccess;

/* Memory management functions. */
int memory_init(Memory *memory, int frame_count);

void memory_destroy(Memory *memory);

int memory_find_page(const Memory *memory,
                     int process_id,
                     int page_number);

int memory_find_free_frame(const Memory *memory);

int memory_touch(Memory *memory,
                 int index,
                 int is_write);

int memory_load_page(Memory *memory,
                     int index,
                     int process_id,
                     int page_number,
                     int is_write);

/* Processes one page reference using Clock. */
int clock_access(Memory *memory,
                 int process_id,
                 int page_number,
                 int is_write,
                 MemoryAccess *result);

#endif
