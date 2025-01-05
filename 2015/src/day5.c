// Day 5 - Doesn't He Have Intern-Elves For This?
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "day5.h"
#include "main.h"

#define INPUT_FILE "data/5.txt"

typedef struct {
    int part1;
    int part2;
    size_t length;
} context;

static const unsigned short bad_strings[] = {0x6162, 0x6364, 0x7071, 0x7879};

static void begin(char *line, void *userData) {
    context *ctx = (context *)userData;
    ctx->length = strlen(line);
}

static void process(char *line, void *userData, size_t lineNum) {
    (void)lineNum;
    context *ctx = (context *)userData;
    int vowelCount = 0;
    for (size_t i = 0; i < ctx->length; ++i) {
        vowelCount += (line[i] == 'a' || line[i] == 'e' || line[i] == 'i' ||
                       line[i] == 'o' || line[i] == 'u');
    }

    bool hasDoubleLetter = false;
    char previous = line[0];
    for (size_t i = 1; i < ctx->length; ++i) {
        if (line[i] == previous) {
            hasDoubleLetter = true;
            break;
        }
        previous = line[i];
    }

    bool hasNoBadString = true;
    for (size_t i = 0; i < ctx->length - 1; ++i) {
        unsigned short c = ((unsigned short)line[i] << 8) | (unsigned short)line[i + 1];
        for (size_t j = 0; j < sizeof(bad_strings) / sizeof(bad_strings[0]); ++j) {
            if (c == bad_strings[j]) {
                hasNoBadString = false;
                goto leave_has_no_bad_string;
            }
        }
    }
leave_has_no_bad_string:;

    bool hasPairTwice = false;
    for (size_t i = 0; i < ctx->length - 3; ++i) {
        for (size_t j = i + 2; j < ctx->length - 1; ++j) {
            if (line[i] == line[j] && line[i + 1] == line[j + 1]) {
                hasPairTwice = true;
                goto leave_has_pair_twice;
            }
        }
    }
leave_has_pair_twice:;

    bool hasSandwich = false;
    for (size_t i = 0; i < ctx->length - 2; ++i) {
        if (line[i] == line[i + 2]) {
            hasSandwich = true;
            break;
        }
    }

    ctx->part1 += (vowelCount >= 3 && hasDoubleLetter && hasNoBadString);
    ctx->part2 += (hasPairTwice && hasSandwich);
}

static void end(char *line, void *userData, size_t lineNum) {
    (void)line;
    (void)userData;
    (void)lineNum;
}

void run_day5() {
    context ctx = {0};

    FILE *file = fopen(INPUT_FILE, "r");
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
        line[strcspn(line, "\r\n")] = '\0'; // Trim newline
        begin(line, &ctx);
        process(line, &ctx, 0);
        end(line, &ctx, 0);
    }

    fclose(file);
    free(line);

    clock_gettime(CLOCK_MONOTONIC, &end_time);
    double elapsed_time = (end_time.tv_sec - start_time.tv_sec) +
                          (end_time.tv_nsec - start_time.tv_nsec) / 1e9;

    printf("Part 1 - First number of strings: %d\n", ctx.part1);
    printf("Part 2 - Second number of string: %d\n", ctx.part2);
    printf("Execution time: %.6f seconds\n", elapsed_time);
}

