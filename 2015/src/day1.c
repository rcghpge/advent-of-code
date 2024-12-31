// Day 1
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "day1.h"
#include "main.h"

#define INPUT_FILE "data/1.txt"

// Solve Part 1: Calculate the final floor
static int solve_part1(const char *input) {
    int floor = 0;
    while (*input) {
        floor += (*input++ == '(') ? 1 : -1;
    }
    return floor;
}

// Solve Part 2: Find the first position where Santa enters the basement
static int solve_part2(const char *input) {
    for (int i = 0, floor = 0; input[i] != '\0'; ++i) {
        floor += (input[i] == '(') ? 1 : -1;
        if (floor == -1) {
            return i + 1;
        }
    }
    return 0; // Should not reach
}

// High-precision timer
static double get_time_in_seconds() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

void run_day1() {
    // Open the input file
    FILE *file = fopen(INPUT_FILE, "r");
    if (!file) {
        perror("Error opening input file");
        return;
    }

    // Determine file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    // Allocate memory for the input and read it
    char *input = malloc(file_size + 1);
    if (!input) {
        perror("Memory allocation failed");
        fclose(file);
        return;
    }

    fread(input, 1, file_size, file);
    input[file_size] = '\0'; // Null-terminate the input
    fclose(file);

    // Time the solution
    double start_time = get_time_in_seconds();
    int part1_result = solve_part1(input);
    int part2_result = solve_part2(input);
    double end_time = get_time_in_seconds();

    // Output the results
    printf("Part 1: Santa ends up on floor %d\n", part1_result);
    printf("Part 2: First basement entry at position %d\n", part2_result);

    // Output execution time
    double elapsed_time = end_time - start_time;
    printf("Execution Time: %.6f seconds\n", elapsed_time);

    // Free allocated memory
    free(input);
}

