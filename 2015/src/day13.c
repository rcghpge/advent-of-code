// Day 13 - Knights of the Dinner Table
#include "main.h"
#include "day13.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <limits.h>

#define MAX_PERSONS 10
#define NAME_LENGTH 16

typedef struct {
    int8_t happiness[MAX_PERSONS];
} person;

typedef struct {
    person persons[MAX_PERSONS];
    uint8_t personCount;
    int32_t happiness;
    char names[MAX_PERSONS][NAME_LENGTH];
} context;

// Helper function to map a name to an index
static uint8_t get_person_index(context *ctx, const char *name) {
    for (uint8_t i = 0; i < ctx->personCount; ++i) {
        if (strcmp(ctx->names[i], name) == 0) {
            return i;
        }
    }
    if (ctx->personCount >= MAX_PERSONS) {
        fprintf(stderr, "Error: Exceeded maximum number of persons.\n");
        exit(EXIT_FAILURE);
    }
    strcpy(ctx->names[ctx->personCount], name);
    return ctx->personCount++;
}

// Parse input and populate the context
static void parse_input(const char *contents, context *ctx) {
    char name1[NAME_LENGTH], name2[NAME_LENGTH], gainOrLose[8];
    int value;
    const char *line = contents;

    while (*line != '\0') {
        if (sscanf(line, "%15s would %7s %d happiness units by sitting next to %15s.",
                   name1, gainOrLose, &value, name2) != 4) {
            fprintf(stderr, "Error: Unexpected input format.\n");
            exit(EXIT_FAILURE);
        }

        if (strcmp(gainOrLose, "lose") == 0) {
            value = -value;
        }

        name2[strcspn(name2, ".")] = '\0';

        uint8_t person1 = get_person_index(ctx, name1);
        uint8_t person2 = get_person_index(ctx, name2);
        ctx->persons[person1].happiness[person2] = value;

        line = strchr(line, '\n');
        if (line) line++;
    }
}

// Calculate total happiness for a permutation
static void calculate_happiness(const size_t *indices, size_t length, context *ctx) {
    int32_t happiness = 0;

    for (size_t i = 0; i < length; ++i) {
        size_t next = (i + 1) % length;
        happiness += ctx->persons[indices[i]].happiness[indices[next]];
        happiness += ctx->persons[indices[next]].happiness[indices[i]];
    }

    if (happiness > ctx->happiness) {
        ctx->happiness = happiness;
    }
}

// Permutation generation using backtracking
static void generate_permutations(size_t *indices, size_t start, size_t end, void (*action)(const size_t *, size_t, context *), context *ctx) {
    if (start == end) {
        action(indices, end, ctx);
        return;
    }

    for (size_t i = start; i < end; ++i) {
        size_t temp = indices[start];
        indices[start] = indices[i];
        indices[i] = temp;

        generate_permutations(indices, start + 1, end, action, ctx);

        indices[i] = indices[start];
        indices[start] = temp;
    }
}

void run_day13() {
    clock_t start_time = clock();

    // Initialize context
    context ctx = {.happiness = INT32_MIN};
    memset(ctx.persons, 0, sizeof(ctx.persons));
    memset(ctx.names, 0, sizeof(ctx.names));

    // Read input data
    const char *input_file = "data/13.txt";
    FILE *file = fopen(input_file, "r");
    if (!file) {
        perror("Failed to open input file");
        exit(EXIT_FAILURE);
    }
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    rewind(file);

    char *contents = malloc(length + 1);
    if (!contents) {
        perror("Failed to allocate memory for input data");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fread(contents, 1, length, file);
    contents[length] = '\0';
    fclose(file);

    // Parse input
    parse_input(contents, &ctx);
    free(contents);

    // Prepare indices for permutations
    size_t indices[MAX_PERSONS];
    for (size_t i = 0; i < ctx.personCount; ++i) {
        indices[i] = i;
    }

    // Solve Part 1
    generate_permutations(indices, 0, ctx.personCount, calculate_happiness, &ctx);
    int32_t part1 = ctx.happiness;

    // Solve Part 2: Add one more person with 0 happiness changes
    ctx.happiness = INT32_MIN;

    // Add yourself as the last person
    for (uint8_t i = 0; i < ctx.personCount; ++i) {
        ctx.persons[ctx.personCount].happiness[i] = 0;
        ctx.persons[i].happiness[ctx.personCount] = 0;
    }

    // Increment person count and update indices
    ctx.personCount++;
    for (size_t i = 0; i < ctx.personCount; ++i) {
        indices[i] = i;
    }

    generate_permutations(indices, 0, ctx.personCount, calculate_happiness, &ctx);

    // Print results
    printf("Part 1 - Total change in happiness 1: %d\n", part1);
    printf("Part 2 - Total change in happiness 2: %d\n", ctx.happiness);

    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Execution time: %.2f seconds\n", elapsed_time);
}

