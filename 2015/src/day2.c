// Day 2 - I Was Told There Would Be No Math
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "day2.h"
#include "main.h"

#define INPUT_FILE "data/2.txt"

// Solve Part 1: Calculate the total wrapping paper required
static int solve_part1(const char *input) {
    int total_paper = 0;
    const char *ptr = input;

    while (*ptr) {
        int l, w, h;
        if (sscanf(ptr, "%dx%dx%d", &l, &w, &h) == 3) {
            int area1 = l * w;
            int area2 = w * h;
            int area3 = h * l;
            int slack = (area1 < area2) ? (area1 < area3 ? area1 : area3) : (area2 < area3 ? area2 : area3);
            total_paper += 2 * (area1 + area2 + area3) + slack;
        }

        // Move to the next line
        while (*ptr && *ptr != '\n') ++ptr;
        if (*ptr == '\n') ++ptr;
    }

    return total_paper;
}

// Solve Part 2: Calculate the total ribbon length required
static int solve_part2(const char *input) {
    int total_ribbon = 0;
    const char *ptr = input;

    while (*ptr) {
        int l, w, h;
        if (sscanf(ptr, "%dx%dx%d", &l, &w, &h) == 3) {
            int perimeter1 = 2 * (l + w);
            int perimeter2 = 2 * (w + h);
            int perimeter3 = 2 * (h + l);
            int smallest_perimeter = (perimeter1 < perimeter2) ? (perimeter1 < perimeter3 ? perimeter1 : perimeter3) : (perimeter2 < perimeter3 ? perimeter2 : perimeter3);
            int volume = l * w * h;
            total_ribbon += smallest_perimeter + volume;
        }

        // Move to the next line
        while (*ptr && *ptr != '\n') ++ptr;
        if (*ptr == '\n') ++ptr;
    }

    return total_ribbon;
}

// High-precision timer
static double get_time_in_seconds() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

void run_day2() {
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
    printf("Part 1 - Total wrapping paper required: %d\n", part1_result);
    printf("Part 2 - Total ribbon length required %d\n", part2_result);

    // Output execution time
    double elapsed_time = end_time - start_time;
    printf("Execution time: %.6f seconds\n", elapsed_time);

    // Free allocated memory
    free(input);
}

