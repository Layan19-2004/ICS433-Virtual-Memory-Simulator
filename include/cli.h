#ifndef CLI_H
#define CLI_H

typedef enum {
    ALGORITHM_FIFO = 1,
    ALGORITHM_LRU,
    ALGORITHM_CLOCK
} Algorithm;

void print_menu(void);
Algorithm choose_algorithm(void);
int read_frame_count(void);
int choose_input_mode(void);

#endif