// Day 18 - Like a GIF For Your Yard
#include "main.h"
#include "day18.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>

#define GRID_SIZE ((100 * 100) >> 3) // 100x100 grid packed into bits

static inline void turn_on_light(uint8_t *lights, uint32_t i) {
    lights[i >> 3] |= (1 << (i & 7));
}

static inline int light_state(const uint8_t *lights, uint32_t i) {
    return (lights[i >> 3] & (1 << (i & 7))) != 0;
}

static inline uint8_t count_neighbors(const uint8_t *grid, int x, int y) {
    uint8_t count = 0;
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dx == 0 && dy == 0) continue;
            int nx = x + dx;
            int ny = y + dy;
            if (nx >= 0 && nx < 100 && ny >= 0 && ny < 100) {
                count += light_state(grid, ny * 100 + nx);
            }
        }
    }
    return count;
}

static void update(const uint8_t *grid, uint8_t *buffer, int corners_on) {
    memset(buffer, 0, GRID_SIZE);
    for (int y = 0; y < 100; y++) {
        for (int x = 0; x < 100; x++) {
            uint32_t idx = y * 100 + x;
            uint8_t neighbors = count_neighbors(grid, x, y);
            if (light_state(grid, idx)) {
                if (neighbors == 2 || neighbors == 3) turn_on_light(buffer, idx);
            } else {
                if (neighbors == 3) turn_on_light(buffer, idx);
            }
        }
    }
    if (corners_on) {
        turn_on_light(buffer, 0);
        turn_on_light(buffer, 99);
        turn_on_light(buffer, 9900);
        turn_on_light(buffer, 9999);
    }
}

static uint32_t count_lights(const uint8_t *grid) {
    uint32_t total = 0;
    for (uint32_t i = 0; i < GRID_SIZE; i++) {
        total += __builtin_popcount(grid[i]); // Bit counting
    }
    return total;
}

static uint32_t solve(const uint8_t *initial_grid, int steps, int corners_on) {
    uint8_t front[GRID_SIZE], back[GRID_SIZE];
    memcpy(front, initial_grid, GRID_SIZE);

    if (corners_on) {
        turn_on_light(front, 0);
        turn_on_light(front, 99);
        turn_on_light(front, 9900);
        turn_on_light(front, 9999);
    }

    for (int step = 0; step < steps; step++) {
        update(front, back, corners_on);
        memcpy(front, back, GRID_SIZE);
    }

    return count_lights(front);
}

static void parse_input(const char *line, uint8_t *grid, int row) {
    for (int col = 0; col < 100; col++) {
        if (line[col] == '#') turn_on_light(grid, row * 100 + col);
    }
}

void run_day18() {
    clock_t start_time = clock();

    uint8_t grid[GRID_SIZE] = {0};
    FILE *file = fopen("data/18.txt", "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[102]; // Adjust buffer size for safety
    int row = 0;
    while (fgets(line, sizeof(line), file)) {
        if (strlen(line) < 100) {
            fprintf(stderr, "Error: Invalid input format on line %d\n", row + 1);
            fclose(file);
            exit(EXIT_FAILURE);
        }
        parse_input(line, grid, row++);
    }
    fclose(file);

    uint32_t part1 = solve(grid, 100, 0); // Part 1: Lights can turn on/off
    uint32_t part2 = solve(grid, 100, 1); // Part 2: Corners are always on

    printf("Part 1 - Number of lights on after 100 steps: %u\n", part1);
    printf("Part 2 - Number of lights on after 100 steps 2: %u\n", part2);

    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Execution time: %.2f seconds\n", elapsed_time);
}

