// Day 25 - Let It Snow
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "main.h"
#include "day25.h"

#define START_CODE 20151125
#define MULTIPLIER 252533
#define MODULUS 33554393
#define INPUT_FILE "data/25.txt"

// Generate code at a given row and column using diagonal traversal pattern
long find_code(int row, int col) {
    int code_row = 1, code_col = 1;
    long code = START_CODE;

    // Traverse diagonally until we reach target (row, col)
    while (code_row != row || code_col != col) {
        if (code_row == 1) {
            code_row = code_col + 1;
            code_col = 1;
        } else {
            code_row -= 1;
            code_col += 1;
        }
        // Update code using formula (multiply by 252533 and take modulo 33554393)
        code = (code * MULTIPLIER) % MODULUS;
    }

    return code;
}

void run_day25(void) {
    FILE *file = fopen(INPUT_FILE, "r");
    if (!file) {
        perror("Error opening input file");
        return;
    }

    int row, col;
    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    // Read each row and column pair from file
    while (fscanf(file, "To continue, please consult the code grid in the manual.  Enter the code at row %d, column %d.", &row, &col) == 2) {
        // Generate code for given row and column
        long code = find_code(row, col);

        // Print code for current position
        printf("The code at row %d, column %d is: %ld\n", row, col, code);
    }

    clock_gettime(CLOCK_MONOTONIC, &end_time);
    double elapsed_time = (end_time.tv_sec - start_time.tv_sec) +
                          (end_time.tv_nsec - start_time.tv_nsec) / 1e9;

    // Execution time
    printf("Execution time: %.6f seconds\n", elapsed_time);

    fclose(file);
}

