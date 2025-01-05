// Day 14 - Reindeer Olympics
#include "main.h"
#include "day14.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define GOAL_TIME 2503
#define MAX_REINDEERS 16

typedef struct {
    uint8_t speed;      // Speed in km/s
    uint8_t duration;   // Flying duration in seconds
    uint8_t rest;       // Rest duration in seconds
    uint32_t distance;  // Total distance traveled
    uint32_t score;     // Total score for Part 2
} reindeer;

// Parse a line of input and populate a reindeer's data
static void parse_reindeer(const char *line, reindeer *r, size_t index) {
    int speed, duration, rest;

    // Use sscanf to extract the values
    int parsed = sscanf(line, "%*s can fly %d km/s for %d seconds, but then must rest for %d seconds.",
                        &speed, &duration, &rest);

    // Validate all fields were successfully parsed
    if (parsed != 3 || speed <= 0 || duration <= 0 || rest <= 0) {
        fprintf(stderr, "Error: Invalid input for reindeer at index %zu\n", index);
        fprintf(stderr, "Input line: %s\n", line);
        exit(EXIT_FAILURE);
    }

    // Assign parsed values to reindeer struct
    r->speed = (uint8_t)speed;
    r->duration = (uint8_t)duration;
    r->rest = (uint8_t)rest;
    r->distance = 0;
    r->score = 0;
}

// Calculate distance traveled by a reindeer in the given time
static uint32_t calculate_distance(const reindeer *r, uint32_t time) {
    uint32_t cycle_time = r->duration + r->rest;  // Total time for one flying+resting cycle
    uint32_t full_cycles = time / cycle_time;    // Number of complete cycles
    uint32_t remaining_time = time % cycle_time; // Remaining time after full cycles

    uint32_t flying_time = full_cycles * r->duration; // Flying time during full cycles
    flying_time += (remaining_time > r->duration) ? r->duration : remaining_time;

    return flying_time * r->speed; // Total distance traveled
}

// Solve Part 1: Find the maximum distance traveled
static uint32_t solve_part1(const reindeer *reindeers, size_t count) {
    uint32_t max_distance = 0;
    for (size_t i = 0; i < count; i++) {
        uint32_t distance = calculate_distance(&reindeers[i], GOAL_TIME);
        if (distance > max_distance) {
            max_distance = distance;
        }
    }
    return max_distance;
}

// Simulate Part 2: Dynamic scoring system
static uint32_t solve_part2(const reindeer *reindeers, size_t count) {
    uint32_t scores[MAX_REINDEERS] = {0};

    for (uint32_t t = 1; t <= GOAL_TIME; t++) {
        uint32_t max_distance = 0;

        // Calculate distances for each reindeer at time `t`
        for (size_t i = 0; i < count; i++) {
            uint32_t distance = calculate_distance(&reindeers[i], t);
            if (distance > max_distance) {
                max_distance = distance;
            }
        }

        // Award points to all reindeers in the lead
        for (size_t i = 0; i < count; i++) {
            if (calculate_distance(&reindeers[i], t) == max_distance) {
                scores[i]++;
            }
        }
    }

    // Find maximum score
    uint32_t max_score = 0;
    for (size_t i = 0; i < count; i++) {
        if (scores[i] > max_score) {
            max_score = scores[i];
        }
    }
    return max_score;
}

void run_day14() {
    clock_t start_time = clock();
    FILE *file = fopen("data/14.txt", "r");
    if (!file) {
        perror("Failed to open input file");
        exit(EXIT_FAILURE);
    }

    reindeer reindeers[MAX_REINDEERS];
    size_t count = 0;
    char line[128];

    // Parse each line of input
    while (fgets(line, sizeof(line), file) && count < MAX_REINDEERS) {
        parse_reindeer(line, &reindeers[count], count);
        count++;
    }
    fclose(file);

    // Solve Part 1 and Part 2
    uint32_t part1 = solve_part1(reindeers, count);
    uint32_t part2 = solve_part2(reindeers, count);

    // Print results
    printf("Part 1 - Total distance: %u\n", part1);
    printf("Part 2 - Total points: %u\n", part2);

    // Print execution time
    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Execution time: %.2f seconds\n", elapsed_time);
}

