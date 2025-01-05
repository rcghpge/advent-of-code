// Day 3 - Perfectly Spherical Houses in a Vacuum
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "day3.h"
#include "main.h"

#define INPUT_FILE "data/3.txt"

// Solve Part 1: Single Santa
static size_t solve_part1(const char *input, size_t length) {
    int visited[1000][1000] = {{0}}; // Assume a grid size
    int x = 500, y = 500; // Start at middle of grid
    visited[x][y] = 1;

    size_t unique_houses = 1;

    for (size_t i = 0; i < length; ++i) {
        switch (input[i]) {
            case '<': x--; break;
            case '>': x++; break;
            case '^': y++; break;
            case 'v': y--; break;
        }

        if (visited[x][y] == 0) {
            unique_houses++;
        }
        visited[x][y]++;
    }

    return unique_houses;
}

// Solve Part 2: Santa and Robo-Santa
static size_t solve_part2(const char *input, size_t length) {
    int visited[1000][1000] = {{0}}; // Assume a grid size
    int santa_x = 500, santa_y = 500;
    int robo_x = 500, robo_y = 500;
    visited[santa_x][santa_y] = 1;

    size_t unique_houses = 1;

    for (size_t i = 0; i < length; ++i) {
        int *x = (i % 2 == 0) ? &santa_x : &robo_x;
        int *y = (i % 2 == 0) ? &santa_y : &robo_y;

        switch (input[i]) {
            case '<': (*x)--; break;
            case '>': (*x)++; break;
            case '^': (*y)++; break;
            case 'v': (*y)--; break;
        }

        if (visited[*x][*y] == 0) {
            unique_houses++;
        }
        visited[*x][*y]++;
    }

    return unique_houses;
}

// High-precision timer
static double get_time_in_seconds() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

void run_day3() {
    // Open input file
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

    // Time solution
    double start_time = get_time_in_seconds();
    size_t part1_result = solve_part1(input, file_size);
    size_t part2_result = solve_part2(input, file_size);
    double end_time = get_time_in_seconds();

    // Output the results
    printf("Part 1: %zu unique houses visited\n", part1_result);
    printf("Part 2: %zu unique houses visited\n", part2_result);

    // Output execution time
    double elapsed_time = end_time - start_time;
    printf("Execution time: %.6f seconds\n", elapsed_time);

    // Free allocated memory
    free(input);
}

