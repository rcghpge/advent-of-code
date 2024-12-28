// Main script
#include <stdio.h>
#include "day1.h"
// Include other day headers as needed

void run_day(int day) {
    char input_file[256];
    snprintf(input_file, sizeof(input_file), "data/%d.txt", day);

    FILE *input = fopen(input_file, "r");
    if (!input) {
        printf("Error: Unable to open file %s\n", input_file);
        return;
    }

    switch (day) {
        case 1:
            solve_day1(input);
            break;
        // Add cases for additional days as needed
        default:
            printf("Day %d not implemented yet.\n", day);
    }

    fclose(input);
}

int main() {
    printf("Advent of Code 2015 Solutions\n\n");

    for (int day = 1; day <= 25; day++) {
        run_day(day);
    }

    return 0;
}

