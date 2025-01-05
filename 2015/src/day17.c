// Day 17 - No Such Thing as Too Much
#include "main.h"
#include "day17.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <time.h>

#define TARGET_CAPACITY 150
#define MAX_CONTAINERS 32

static uint8_t container_sizes[MAX_CONTAINERS];
static uint8_t count = 0;

// Parse each line of input to populate container sizes
static void parse_container(const char *line) {
    if (count >= MAX_CONTAINERS) {
        fprintf(stderr, "Error: Too many containers (max %d).\n", MAX_CONTAINERS);
        exit(EXIT_FAILURE);
    }
    container_sizes[count++] = (uint8_t)strtoul(line, NULL, 10);
}

// Solve both parts of problem
static void solve(uint32_t *part1, uint32_t *part2) {
    uint32_t total_combinations = 1U << count; // 2^count
    uint32_t valid_combinations = 0, min_container_count = UINT32_MAX, min_count_combinations = 0;

    for (uint32_t i = 0; i < total_combinations; i++) {
        uint16_t capacity = 0;
        uint8_t container_count = 0;

        for (uint8_t j = 0; j < count; j++) {
            if (i & (1U << j)) { // Check if j-th container is used
                capacity += container_sizes[j];
                container_count++;
            }
        }

        if (capacity == TARGET_CAPACITY) {
            valid_combinations++;
            if (container_count < min_container_count) {
                min_container_count = container_count;
                min_count_combinations = 1;
            } else if (container_count == min_container_count) {
                min_count_combinations++;
            }
        }
    }

    *part1 = valid_combinations;
    *part2 = min_count_combinations;
}

void run_day17() {
    clock_t start_time = clock();

    FILE *file = fopen("data/17.txt", "r");
    if (!file) {
        perror("Failed to open input file");
        exit(EXIT_FAILURE);
    }

    char line[16];
    while (fgets(line, sizeof(line), file)) {
        parse_container(line);
    }
    fclose(file);

    uint32_t part1 = 0, part2 = 0;
    solve(&part1, &part2);

    printf("Part 1 - Number of combinations: %u\n", part1);
    printf("Part 2 - Number of methods: %u\n", part2);

    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Execution time: %.2f seconds\n", elapsed_time);
}

