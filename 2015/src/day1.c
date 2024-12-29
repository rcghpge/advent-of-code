// Day 1
#include <stdio.h>
#include <stdlib.h>
#include "day1.h"
#include "main.h"

#define INPUT_FILE "data/1.txt"

void run_day1() {
    // Marker to indicate Day 1 execution
    printf("=== Day 1: Running Solution ===\n");

    // Open the input file
    FILE *file = fopen(INPUT_FILE, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open input file %s\n", INPUT_FILE);
        return;
    }

    // Initialize result (floor Santa ends up on)
    int result = 0;
    char ch;

    // Read and process input instructions
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '(') {
            result++; // Up one floor
        } else if (ch == ')') {
            result--; // Down one floor
        } else if (ch != '\n' && ch != '\r') {
            // Ignore invalid characters but warn if found
            fprintf(stderr, "Warning: Invalid character '%c' in input file, ignoring it.\n", ch);
        }
    }

    // Close the file
    fclose(file);

    // Output the result
    printf("Day 1 Solution: Santa ends up on floor %d\n", result);
}


