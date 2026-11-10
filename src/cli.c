#include "cli.h"
#include <stdio.h>

static int read_int_in_range(const char *prompt, int min, int max) {
    int value;
    int character;

    while (1) {
        printf("%s", prompt);

        if (scanf("%d", &value) != 1) {
            printf("Please enter a valid number.\n");

            while ((character = getchar()) != '\n' &&
                   character != EOF) {
                /* Clear invalid input */
            }

            continue;
        }

        while ((character = getchar()) != '\n' &&
               character != EOF) {
            /* Clear remaining input */
        }

        if (value >= min && value <= max) {
            return value;
        }

        printf("Please enter a value between %d and %d.\n",
               min, max);
    }
}

void print_menu(void) {
    printf("\n=== Virtual Memory Simulator ===\n");
    printf("1. FIFO\n");
    printf("2. LRU\n");
    printf("3. Clock\n");
}

Algorithm choose_algorithm(void) {
    int choice;

    print_menu();

    choice = read_int_in_range(
        "Choose a page-replacement algorithm: ", 1, 3);

    return (Algorithm)choice;
}

int read_frame_count(void) {
    return read_int_in_range(
        "Enter the number of frames: ", 1, 1000);
}

int choose_input_mode(void) {
    printf("\nInput options:\n");
    printf("1. Enter references manually\n");
    printf("2. Load references from a file\n");

    return read_int_in_range(
        "Choose an input method: ", 1, 2);
}