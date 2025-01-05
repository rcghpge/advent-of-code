// Day 16 - Aunt Sue
#include "main.h"
#include "day16.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

typedef enum {
    PROP_CHILDREN,
    PROP_SAMOYEDS,
    PROP_AKITAS,
    PROP_VIZLAS,
    PROP_CARS,
    PROP_PERFUMES,
    PROP_CATS,
    PROP_TREES,
    PROP_POMERANIANS,
    PROP_GOLDFISH,
    PROP_COUNT,
} property;

typedef struct {
    int16_t properties[PROP_COUNT];
} aunt;

#define MAX_AUNTS 500

static aunt aunts[MAX_AUNTS];
static size_t aunt_count = 0;

// Known properties of Aunt Sue being searched
static const aunt search = {{3, 2, 0, 0, 2, 1, 7, 3, 3, 5}};

// Parse a single property from input line
static void parse_property(const char *line, const char **out, aunt *a) {
    switch (line[0]) {
        case 'a': // "akitas"
            a->properties[PROP_AKITAS] = (int16_t)strtol(line + 8, (char **)out, 10);
            break;
        case 'c': // "cars", "cats", "children"
            switch (line[2]) {
                case 'r': // "cars"
                    a->properties[PROP_CARS] = (int16_t)strtol(line + 6, (char **)out, 10);
                    break;
                case 't': // "cats"
                    a->properties[PROP_CATS] = (int16_t)strtol(line + 6, (char **)out, 10);
                    break;
                case 'i': // "children"
                    a->properties[PROP_CHILDREN] = (int16_t)strtol(line + 10, (char **)out, 10);
                    break;
            }
            break;
        case 'g': // "goldfish"
            a->properties[PROP_GOLDFISH] = (int16_t)strtol(line + 10, (char **)out, 10);
            break;
        case 'p': // "perfumes", "pomeranians"
            if (line[1] == 'e') { // "perfumes"
                a->properties[PROP_PERFUMES] = (int16_t)strtol(line + 10, (char **)out, 10);
            } else { // "pomeranians"
                a->properties[PROP_POMERANIANS] = (int16_t)strtol(line + 13, (char **)out, 10);
            }
            break;
        case 's': // "samoyeds"
            a->properties[PROP_SAMOYEDS] = (int16_t)strtol(line + 10, (char **)out, 10);
            break;
        case 't': // "trees"
            a->properties[PROP_TREES] = (int16_t)strtol(line + 7, (char **)out, 10);
            break;
        case 'v': // "vizslas"
            a->properties[PROP_VIZLAS] = (int16_t)strtol(line + 9, (char **)out, 10);
            break;
    }
}

// Parseline of input and populate `aunts` array
static void parse_aunt(const char *line) {
    if (aunt_count >= MAX_AUNTS) {
        fprintf(stderr, "Error: Too many aunts (max %d).\n", MAX_AUNTS);
        exit(EXIT_FAILURE);
    }

    aunt *a = &aunts[aunt_count];
    memset(a, -1, sizeof(*a)); // Initialize all properties to -1

    // Skip name part
    while (*line != ':') line++;
    line += 2;

    // Parse up to properties
    for (int i = 0; i < 3; i++) {
        parse_property(line, &line, a);
        if (*line == ',') line += 2; // Skip ", "
    }

    aunt_count++;
}

// Property comparison functions
static uint8_t prop_score(const aunt *a, property p) {
    return a->properties[p] == search.properties[p] || a->properties[p] == -1;
}

static uint8_t prop_score_gt(const aunt *a, property p) {
    return a->properties[p] > search.properties[p] || a->properties[p] == -1;
}

static uint8_t prop_score_lt(const aunt *a, property p) {
    return a->properties[p] < search.properties[p] || a->properties[p] == -1;
}

// Scoring functions for Part 1 and Part 2
static uint8_t (*part1_funcs[PROP_COUNT])(const aunt *, property) = {
    [PROP_CHILDREN] = prop_score,    [PROP_SAMOYEDS] = prop_score,
    [PROP_AKITAS] = prop_score,      [PROP_VIZLAS] = prop_score,
    [PROP_CARS] = prop_score,        [PROP_PERFUMES] = prop_score,
    [PROP_CATS] = prop_score,        [PROP_TREES] = prop_score,
    [PROP_POMERANIANS] = prop_score, [PROP_GOLDFISH] = prop_score,
};

static uint8_t (*part2_funcs[PROP_COUNT])(const aunt *, property) = {
    [PROP_CHILDREN] = prop_score,       [PROP_SAMOYEDS] = prop_score,
    [PROP_AKITAS] = prop_score,         [PROP_VIZLAS] = prop_score,
    [PROP_CARS] = prop_score,           [PROP_PERFUMES] = prop_score,
    [PROP_CATS] = prop_score_gt,        [PROP_TREES] = prop_score_gt,
    [PROP_POMERANIANS] = prop_score_lt, [PROP_GOLDFISH] = prop_score_lt,
};

// Solve the problem given a scoring function
static uint16_t solve(uint8_t (*funcs[PROP_COUNT])(const aunt *, property)) {
    uint16_t result = 0;
    uint8_t max_score = 0;

    for (size_t i = 0; i < aunt_count; i++) {
        uint8_t score = 0;

        for (property p = 0; p < PROP_COUNT; p++) {
            score += funcs[p](&aunts[i], p);
        }

        if (score > max_score) {
            max_score = score;
            result = i + 1; // Aunt Sue numbers are 1-indexed
        }
    }

    return result;
}

void run_day16() {
    clock_t start_time = clock();
    FILE *file = fopen("data/16.txt", "r");
    if (!file) {
        perror("Failed to open input file");
        exit(EXIT_FAILURE);
    }

    char line[128];
    while (fgets(line, sizeof(line), file)) {
        parse_aunt(line);
    }
    fclose(file);

    uint16_t part1 = solve(part1_funcs);
    uint16_t part2 = solve(part2_funcs);

    printf("Part 1 - Sue number: %u\n", part1);
    printf("Part 2 - Real Aunt Sue number: %u\n", part2);

    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Execution time: %.2f seconds\n", elapsed_time);
}

