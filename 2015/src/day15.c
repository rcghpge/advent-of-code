// Day 15 - Science for Hungry People
#include "main.h"
#include "day15.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define MAX_INGREDIENTS 4

typedef struct {
    int8_t capacity;
    int8_t durability;
    int8_t flavor;
    int8_t texture;
    int8_t calories;
} ingredient;

ingredient ingredients[MAX_INGREDIENTS];
uint8_t count = 0;

// Parse a line of input and populate ingredient's properties
static void parse_ingredient(const char *line) {
    if (count >= MAX_INGREDIENTS) {
        fprintf(stderr, "Error: Too many ingredients (max %d).\n", MAX_INGREDIENTS);
        exit(EXIT_FAILURE);
    }

    ingredient *i = &ingredients[count];

    // Use sscanf to extract ingredient properties
    int parsed = sscanf(line,
                        "%*[^:]: capacity %hhd, durability %hhd, flavor %hhd, texture %hhd, calories %hhd",
                        &i->capacity, &i->durability, &i->flavor, &i->texture, &i->calories);

    // Validate parsing success
    if (parsed != 5) {
        fprintf(stderr, "Error: Invalid ingredient format.\n");
        fprintf(stderr, "Input line: %s\n", line);
        exit(EXIT_FAILURE);
    }

    count++;
}

// Clamp negative values to zero
static inline int32_t zero_clamp(int32_t n) {
    return n > 0 ? n : 0;
}

// Calculate the score of a recipe
static int32_t calc_score(const uint8_t amounts[MAX_INGREDIENTS]) {
    int32_t capacity = 0, durability = 0, flavor = 0, texture = 0;

    for (uint8_t i = 0; i < count; i++) {
        capacity += ingredients[i].capacity * amounts[i];
        durability += ingredients[i].durability * amounts[i];
        flavor += ingredients[i].flavor * amounts[i];
        texture += ingredients[i].texture * amounts[i];
    }

    return zero_clamp(capacity) * zero_clamp(durability) * zero_clamp(flavor) * zero_clamp(texture);
}

// Calculate the total calories of a recipe
static inline int32_t calc_calories(const uint8_t amounts[MAX_INGREDIENTS]) {
    int32_t calories = 0;

    for (uint8_t i = 0; i < count; i++) {
        calories += ingredients[i].calories * amounts[i];
    }

    return calories;
}

// Calculate the score for recipes with calorie constraint
static inline int32_t calc_score_with_calories(const uint8_t amounts[MAX_INGREDIENTS]) {
    return (calc_calories(amounts) == 500) ? calc_score(amounts) : 0;
}

// Solve for the best score given a scoring function
static int32_t solve(int32_t (*calc)(const uint8_t[MAX_INGREDIENTS])) {
    uint8_t amounts[MAX_INGREDIENTS];
    int32_t best_score = 0;

    // Try all combinations of amounts summing to 100
    for (uint8_t i = 0; i <= 100; i++) {
        amounts[0] = i;
        for (uint8_t j = 0; j <= 100 - i; j++) {
            amounts[1] = j;
            for (uint8_t k = 0; k <= 100 - i - j; k++) {
                amounts[2] = k;
                amounts[3] = 100 - i - j - k;

                int32_t score = calc(amounts);
                if (score > best_score) {
                    best_score = score;
                }
            }
        }
    }

    return best_score;
}

// Framework-compatible function to execute Day 15 solution
void run_day15() {
    clock_t start_time = clock();
    FILE *file = fopen("data/15.txt", "r");
    if (!file) {
        perror("Failed to open input file");
        exit(EXIT_FAILURE);
    }

    char line[128];
    while (fgets(line, sizeof(line), file)) {
        parse_ingredient(line);
    }
    fclose(file);

    // Solve Part 1 and Part 2
    int32_t part1 = solve(calc_score);
    int32_t part2 = solve(calc_score_with_calories);

    // Print results
    printf("Part 1 - Score 1: %d\n", part1);
    printf("Part 2 - Score 2: %d\n", part2);

    // Print execution time
    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Execution time: %.2f seconds\n", elapsed_time);
}

