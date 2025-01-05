// Day 8 - Matchsticks
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include "day8.h"
#include "main.h"

typedef struct {
    size_t visual, memory, encoded;
} context;

static void parse_line(const char *line, context *ctx) {
    size_t visual_length = strlen(line);
    ctx->visual += visual_length;
    ctx->encoded += 2;

    for (size_t i = 0; i < visual_length; i++) {
        if (line[i] == '\\') {
            if (line[i + 1] == '\\' || line[i + 1] == '\"') {
                ctx->memory += 1;
                ctx->encoded += 4; // Encoded adds \\ or \"
                i++; // Skip next character
            } else if (line[i + 1] == 'x' && i + 3 < visual_length &&
                       isxdigit(line[i + 2]) && isxdigit(line[i + 3])) {
                ctx->memory += 1;
                ctx->encoded += 5; // Encoded adds \\xNN
                i += 3; // Skip the \xNN sequence
            } else {
                ctx->memory += 1;
                ctx->encoded += 2; // Encoded adds \\ for single backslash
            }
        } else if (line[i] == '\"') {
            ctx->encoded += 2; // Encoded adds \"
        } else {
            ctx->memory += 1;
            ctx->encoded += 1; // Normal characters
        }
    }
}

void run_day8() {
    context ctx = {0};

    FILE *file = fopen("data/8.txt", "r");
    if (!file) {
        perror("Error opening input file");
        return;
    }

    clock_t start_time = clock();

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        size_t len = strlen(line);
        if (line[len - 1] == '\n') {
            line[len - 1] = '\0'; // Remove newline
        }
        parse_line(line, &ctx);
    }
    fclose(file);

    clock_t end_time = clock();
    double execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Part 1: %zu\n", ctx.visual - ctx.memory);
    printf("Part 2: %zu\n", ctx.encoded - ctx.visual);
    printf("Execution time: %.6f seconds\n", execution_time);
}

