// Day 1
#include <stdio.h>
#include "day1.h"

void solve_day1(FILE *input) {
    // Marker to indicate Day 1 execution
    printf("=== Day 1: Running Solution ===\n");

    // Example logic to parse input and solve the problem
    int result = 0;
    char ch;

    while ((ch = fgetc(input)) != EOF) {
        if (ch == '(') {
            result++;
        } else if (ch == ')') {
            result--;
        }
    }

    // Output the result
    printf("Day 1 Solution: %d\n", result);
}

