// Main script
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "day1.h"

// Include other day headers as needed
// #include "day2.h"
// #include "day3.h"
// ...

void run_day(int day) {
    printf("Running solution for Day %d...\n", day);

    // Map days to their respective solution functions
    switch (day) {
        case 1:
            run_day1(); // Function for Day 1 solution
            break;
        // Add other cases as needed
        // case 2:
        //     day2_solution();
        //     break;
        // ...
        default:
            fprintf(stderr, "Error: No solution implemented for Day %d\n", day);
            break;
    }
}

void run_all_days() {
    printf("Running solutions for all days...\n");

    for (int day = 1; day <= 25; day++) {
        run_day(day);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <day> | all\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "all") == 0) {
        run_all_days();
    } else {
        int day = atoi(argv[1]);
        if (day < 1 || day > 25) {
            fprintf(stderr, "Error: Day %d is out of range (1-25).\n", day);
            return 1;
        }
        run_day(day);
    }

    return 0;
}

