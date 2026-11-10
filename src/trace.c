#include "trace.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int load_trace_from_file(const char *filename, Trace *trace) {
    FILE *file;
    int page_number;
    int process_id;
    char operation;
    int capacity = 10;

    if (filename == NULL || trace == NULL) {
        return 0;
    }

    file = fopen(filename, "r");
    if (file == NULL) {
        return 0;
    }

    trace->references = malloc(capacity * sizeof(PageReference));
    trace->count = 0;

    if (trace->references == NULL) {
        fclose(file);
        return 0;
    }

    while (fscanf(file, "%d %d %c",
                  &page_number, &process_id, &operation) == 3) {

        if (trace->count == capacity) {
            capacity *= 2;

            PageReference *new_references =
                realloc(trace->references,
                        capacity * sizeof(PageReference));

            if (new_references == NULL) {
                free(trace->references);
                trace->references = NULL;
                trace->count = 0;
                fclose(file);
                return 0;
            }

            trace->references = new_references;
        }

        trace->references[trace->count].page_number = page_number;
        trace->references[trace->count].process_id = process_id;

        operation = (char)toupper((unsigned char)operation);

        if (operation == 'W') {
            trace->references[trace->count].operation = OP_WRITE;
        } else {
            trace->references[trace->count].operation = OP_READ;
        }

        trace->count++;
    }

    fclose(file);
    return 1;
}

void free_trace(Trace *trace) {
    if (trace != NULL) {
        free(trace->references);
        trace->references = NULL;
        trace->count = 0;
    }
}

void print_trace(const Trace *trace) {
    int i;

    if (trace == NULL) {
        return;
    }

    for (i = 0; i < trace->count; i++) {
        printf("Page: %d | Process: %d | Operation: %s\n",
               trace->references[i].page_number,
               trace->references[i].process_id,
               trace->references[i].operation == OP_WRITE
                   ? "WRITE"
                   : "READ");
    }
}