// Day 9 - All in a Single Night
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include "day8.h"
#include "main.h"

#define MAX_LOCATIONS 100
#define MAX_NAME_LENGTH 32

typedef unsigned int u32;

typedef struct {
    u32 distances[MAX_LOCATIONS];
} location;

typedef struct {
    location locations[MAX_LOCATIONS];
    unsigned char locationCount;
    u32 shortest;
    u32 longest;
} context;

static void parse_line(char *line, context *ctx) {
    static char locationIds[MAX_LOCATIONS][MAX_NAME_LENGTH] = {""};
    static int nextLocationId = 0;

    char loc1[MAX_NAME_LENGTH], loc2[MAX_NAME_LENGTH];
    int distance;

    if (sscanf(line, "%s to %s = %d", loc1, loc2, &distance) != 3) {
        fprintf(stderr, "Error: Invalid input format: %s\n", line);
        exit(EXIT_FAILURE);
    }

    int startIdx = -1, destIdx = -1;
    for (int i = 0; i < nextLocationId; i++) {
        if (strcmp(loc1, locationIds[i]) == 0) startIdx = i;
        if (strcmp(loc2, locationIds[i]) == 0) destIdx = i;
    }

    if (startIdx == -1) {
        if (nextLocationId >= MAX_LOCATIONS) {
            fprintf(stderr, "Error: Too many locations (max %d)\n", MAX_LOCATIONS);
            exit(EXIT_FAILURE);
        }
        startIdx = nextLocationId;
        strncpy(locationIds[nextLocationId++], loc1, MAX_NAME_LENGTH - 1);
        ctx->locationCount++;
    }

    if (destIdx == -1) {
        if (nextLocationId >= MAX_LOCATIONS) {
            fprintf(stderr, "Error: Too many locations (max %d)\n", MAX_LOCATIONS);
            exit(EXIT_FAILURE);
        }
        destIdx = nextLocationId;
        strncpy(locationIds[nextLocationId++], loc2, MAX_NAME_LENGTH - 1);
        ctx->locationCount++;
    }

    ctx->locations[startIdx].distances[destIdx] = distance;
    ctx->locations[destIdx].distances[startIdx] = distance;
}

void swap(size_t *a, size_t *b) {
    size_t temp = *a;
    *a = *b;
    *b = temp;
}

void permute(size_t *indices, size_t start, size_t end, context *ctx) {
    if (start == end) {
        u32 distance = 0;
        for (size_t i = 0; i < end - 1; i++) {
            distance += ctx->locations[indices[i]].distances[indices[i + 1]];
        }
        if (distance < ctx->shortest) ctx->shortest = distance;
        if (distance > ctx->longest) ctx->longest = distance;
        return;
    }

    for (size_t i = start; i < end; i++) {
        swap(&indices[start], &indices[i]);
        permute(indices, start + 1, end, ctx);
        swap(&indices[start], &indices[i]);
    }
}

void run_day9(void) {
    clock_t start_time = clock();

    context ctx = {
        .locationCount = 0,
        .shortest = UINT32_MAX,
        .longest = 0
    };

    // Initialize distances to 0
    for (int i = 0; i < MAX_LOCATIONS; i++) {
        for (int j = 0; j < MAX_LOCATIONS; j++) {
            ctx.locations[i].distances[j] = 0;
        }
    }

    FILE *file = fopen("data/9.txt", "r");
    if (!file) {
        perror("Failed to open input file");
        exit(EXIT_FAILURE);
    }

    char line[128];
    while (fgets(line, sizeof(line), file)) {
        parse_line(line, &ctx);
    }
    fclose(file);

    if (ctx.locationCount < 2) {
        fprintf(stderr, "Error: Not enough locations to calculate routes\n");
        exit(EXIT_FAILURE);
    }

    size_t indices[MAX_LOCATIONS];
    for (size_t i = 0; i < ctx.locationCount; i++) {
        indices[i] = i;
    }

    permute(indices, 0, ctx.locationCount, &ctx);

    printf("Part 1: Shortest route: %u\n", ctx.shortest);
    printf("Part 2: Longest route: %u\n", ctx.longest);

    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Execution time: %.2f seconds\n", elapsed_time);
}

