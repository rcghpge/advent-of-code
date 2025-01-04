// Main script
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "main.h"
#include "day1.h"
#include "day2.h"
#include "day3.h"
#include "day4.h"

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
	case 2:
            run_day2();
            break;
	case 3:
	    run_day3();
	    break;
	case 4:
	    run_day4();
	    break;
        // Add other cases as needed
        // case 2:
        //     run_day2();
        //     break;
        default:
            fprintf(stderr, "Error: No solution implemented for Day %d\n", day);
            break;
    }

    printf("Completed solution for Day %d.\n", day);
}

void run_all_days() {
    printf("Running solutions for all implemented days...\n");

    int implemented_days[] = {1, 2, 3, 4}; // Add more days as they are implemented
    int num_days = sizeof(implemented_days) / sizeof(implemented_days[0]);

    for (int i = 0; i < num_days; i++) {
        int day = implemented_days[i];
        printf("Running solution for Day %d (%d/%d)...\n", day, i + 1, num_days);
        run_day(day);
    }

    printf("Completed all implemented days.\n");
}

int main(int argc, char *argv[]) {
    // Validate input arguments
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <day> | all\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Handle "all" argument
    if (strcmp(argv[1], "all") == 0) {
        run_all_days();
        return EXIT_SUCCESS;
    }

    // Parse and validate day argument
    char *endptr;
    long day = strtol(argv[1], &endptr, 10);

    if (*endptr != '\0' || day < 1 || day > 25) {
        fprintf(stderr, "Error: Invalid day '%s'. Please enter a day between 1 and 25.\n", argv[1]);
        return EXIT_FAILURE;
    }

    // Run solution for the specified day
    run_day((int)day);

    return EXIT_SUCCESS;
}

