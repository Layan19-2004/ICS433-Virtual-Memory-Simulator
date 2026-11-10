#ifndef TRACE_H
#define TRACE_H

typedef enum {
    OP_READ,
    OP_WRITE
} Operation;

typedef struct {
    int page_number;
    int process_id;
    Operation operation;
} PageReference;

typedef struct {
    PageReference *references;
    int count;
} Trace;

int load_trace_from_file(const char *filename, Trace *trace);
void free_trace(Trace *trace);
void print_trace(const Trace *trace);

#endif