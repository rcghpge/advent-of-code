// Day 6 - Probably a Fire Hazard
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "day6.h"
#include "main.h"

#define GRID_SIZE 1000
static int grid[GRID_SIZE * GRID_SIZE] = {0};
static int brightness[GRID_SIZE * GRID_SIZE] = {0};

typedef enum {
    INSTRUCTION_TYPE_TURN_OFF,
    INSTRUCTION_TYPE_TURN_ON,
    INSTRUCTION_TYPE_TOGGLE,
} instruction_type;

static const int brightness_changes[] = {-1, 1, 2};

typedef struct {
    instruction_type type;
    int from_x, from_y, to_x, to_y;
} instruction;

static void parse(char *line, instruction *instructions, size_t *count) {
    instruction instr = {0};

    // Determine instruction type
    if (strncmp(line, "turn on", 7) == 0) {
        instr.type = INSTRUCTION_TYPE_TURN_ON;
    } else if (strncmp(line, "turn off", 8) == 0) {
        instr.type = INSTRUCTION_TYPE_TURN_OFF;
    } else if (strncmp(line, "toggle", 6) == 0) {
        instr.type = INSTRUCTION_TYPE_TOGGLE;
    }

    // Parse coordinates
    char *ptr = line + (instr.type == INSTRUCTION_TYPE_TOGGLE ? 7 : 8);
    instr.from_x = strtol(ptr, &ptr, 10);
    instr.from_y = strtol(ptr + 1, &ptr, 10);
    instr.to_x = strtol(ptr + 9, &ptr, 10);
    instr.to_y = strtol(ptr + 1, NULL, 10);

    instructions[*count] = instr;
    (*count)++;
}

static inline void run_instruction(const instruction *const instr) {
    for (int y = instr->from_y; y <= instr->to_y; ++y) {
        for (int x = instr->from_x; x <= instr->to_x; ++x) {
            int idx = y * GRID_SIZE + x;

            // Update grid state
            if (instr->type == INSTRUCTION_TYPE_TURN_OFF) {
                grid[idx] = 0;
            } else if (instr->type == INSTRUCTION_TYPE_TURN_ON) {
                grid[idx] = 1;
            } else if (instr->type == INSTRUCTION_TYPE_TOGGLE) {
                grid[idx] = !grid[idx];
            }

            // Update brightness
            brightness[idx] += brightness_changes[instr->type];
            if (brightness[idx] < 0) brightness[idx] = 0;
        }
    }
}

static void solve(const instruction *instructions, size_t count) {
    for (size_t i = 0; i < count; ++i) {
        run_instruction(&instructions[i]);
    }

    int activeLights = 0;
    int totalBrightness = 0;

    for (int i = 0; i < GRID_SIZE * GRID_SIZE; ++i) {
        activeLights += grid[i];
        totalBrightness += brightness[i];
    }

    printf("Part 1 - Number of lights lit: %d\n", activeLights);
    printf("Part 2 - Total brightness: %d\n", totalBrightness);
}

void run_day6() {
    instruction instructions[10000]; // Ensure large enough array
    size_t instruction_count = 0;

    FILE *file = fopen("data/6.txt", "r");
    if (!file) {
        perror("Error opening input file");
        return;
    }

    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, file)) != -1) {
        parse(line, instructions, &instruction_count);
    }
    fclose(file);
    free(line);

    solve(instructions, instruction_count);

    clock_gettime(CLOCK_MONOTONIC, &end_time);
    double elapsed_time = (end_time.tv_sec - start_time.tv_sec) +
                          (end_time.tv_nsec - start_time.tv_nsec) / 1e9;

    printf("Execution time: %.6f seconds\n", elapsed_time);
}

