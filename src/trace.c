#include "trace.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

static void clear_input(void) {
    int character;

    while ((character = getchar()) != '\n' &&
           character != EOF) {
    }
}

static Operation convert_operation(char operation) {
    operation = (char)toupper((unsigned char)operation);

    if (operation == 'W') {
        return OP_WRITE;
    }

    return OP_READ;
}

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

    trace->references =
        malloc(capacity * sizeof(PageReference));
    trace->count = 0;

    if (trace->references == NULL) {
        fclose(file);
        return 0;
    }

    while (fscanf(file, "%d %d %c",
                  &page_number,
                  &process_id,
                  &operation) == 3) {

        if (trace->count == capacity) {
            PageReference *new_references;

            capacity *= 2;

            new_references =
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

        trace->references[trace->count].page_number =
            page_number;

        trace->references[trace->count].process_id =
            process_id;

        trace->references[trace->count].operation =
            convert_operation(operation);

        trace->count++;
    }

    fclose(file);
    return 1;
}

int enter_trace_manually(Trace *trace) {
    int count;
    int i;
    char operation;

    if (trace == NULL) {
        return 0;
    }

    printf("Enter number of page references: ");

    if (scanf("%d", &count) != 1 || count <= 0) {
        clear_input();
        return 0;
    }

    clear_input();

    trace->references =
        malloc(count * sizeof(PageReference));
    trace->count = count;

    if (trace->references == NULL) {
        trace->count = 0;
        return 0;
    }

    for (i = 0; i < count; i++) {
        printf("Reference %d (page process R/W): ",
               i + 1);

        while (scanf("%d %d %c",
                     &trace->references[i].page_number,
                     &trace->references[i].process_id,
                     &operation) != 3) {
            printf("Invalid input. Use: page process R/W\n");
            clear_input();
            printf("Reference %d (page process R/W): ",
                   i + 1);
        }

        clear_input();

        trace->references[i].operation =
            convert_operation(operation);
    }

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