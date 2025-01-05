// Day 20 - Infinite Elves and Infinite Houses
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "day20.h"
#include "main.h"

#define MAX 1000000

static int solve(int input, int steps, int multiplier) {
    int *presents = calloc(MAX, sizeof(int));
    if (!presents) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < MAX; i++) {
        int delivery_limit = (steps == MAX) ? MAX : steps * i;
        for (int j = i; j < MAX && j <= delivery_limit; j += i) {
            presents[j] += multiplier * i;
        }
    }

    for (int i = 1; i < MAX; i++) {
        if (presents[i] >= input) {
            free(presents);
            return i;
        }
    }

    free(presents);
    return -1;
}

void run_day20() {
    FILE *file = fopen("data/20.txt", "r");
    if (!file) {
        perror("Failed to open input file");
        return;
    }

    int input;
    if (fscanf(file, "%d", &input) != 1) {
        fprintf(stderr, "Failed to read input value\n");
        fclose(file);
        return;
    }
    fclose(file);

    clock_t start_time = clock();

    int part1 = solve(input, MAX, 10);
    int part2 = solve(input, 50, 11);

    clock_t end_time = clock();
    double execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Part 1 - Lowest house number: %d\n", part1);
    printf("Part 2 - Lowest house number 2: %d\n", part2);
    printf("Execution time: %.2f seconds\n", execution_time);
}

